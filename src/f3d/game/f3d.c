#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#include <f3d/engine/engine.h>

#include <f3d/game/game.h>
#include <f3d/engine/shadows.h>

#include <GL/glew.h>
#include <GL/gl.h>


#define FRAMERATE_CAP 60

#define MOUSE_SPEED 0.07

void load_models();
int on_draw(void *arg);
void check_mouse(double xrel, double yrel);
void check_event(SDL_Event *event);
int render_scene(void *arg);
int on_end(void *arg);

window_t window;
material_t *brick, *stone;
object_t *wall, *level, *box;
light_t *light;

vector3f_t velocity;

camera_t camera;

unsigned arrayid;
shader_t shader_main, shader_depth;

int init(void) {
    game_init();
    game_info.flags |= GAME_IS_RUNNING;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    window = window_new("Ethan's 3D Engine", 700, 700, 0);
    default_window = &window;
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_msg(LOG_FATAL, "Could not initialize GLEW\n", 0);
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // enable vsync
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    glGenVertexArrays(1, &arrayid);
    glBindVertexArray(arrayid);
    return 0;
}
 
void init_gl() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glViewport(0, 0, window.width, window.height);
    
    shader_main = shaders_link(
        "Main",
        shader_load("../shaders/m_vert.glsl", SHADER_VERTEX),
        shader_load("../shaders/m_frag.glsl", SHADER_FRAGMENT)
    );
    
    shader_depth = shaders_link(
        "Depth",
        shader_load("../shaders/depth_vert.glsl", SHADER_VERTEX),
        shader_load("../shaders/depth_frag.glsl", SHADER_FRAGMENT)
    );
    
    shader_use(&shader_main);

    camera = camera_new();
    camera.move_speed = 6.0f;
    camera.position = (vector3f_t){2, 3, -5};
    
    load_models();
    
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    
    glEnable(GL_FRAMEBUFFER_SRGB);
    
    // fix overlapping polygons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    
    // disable mouse cursor
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);    
}

void setup_handles() {
    handle_set(HANDLE_END, &on_end);
    handle_set(HANDLE_DRAW, &on_draw);
    handle_set(HANDLE_RENDER_MESHES, &render_scene);
}

int move() {
    int moved = 0;
    
    const float max_velocity = 1.0f;
    
    if (keys_pressed[CONTROL_FORWARD]) {
        velocity.z = max_velocity;
        moved = 1;
    }
    if (keys_pressed[CONTROL_BACKWARD]) {
        velocity.z = -max_velocity;
        moved = 1;
    }
    if (keys_pressed[CONTROL_LEFT]) {
        velocity.x = -max_velocity;
        moved = 1;
    }
    if (keys_pressed[CONTROL_RIGHT]) {
        velocity.x = max_velocity;
    }
    
    if (keys_pressed[CONTROL_1]) {
        if (stone->use_normals != true) {
            stone->use_normals = true;
            material_update(stone, &shader_main);        
        }
    }
    else if (keys_pressed[CONTROL_2]) {
        if (stone->use_normals != false) {
            stone->use_normals = false;
            material_update(stone, &shader_main);        
        }
    }
    
    const float friction = 0.05;
    if (velocity.x) {
        if (velocity.x > 0.0f) {
            camera_move(selected_camera, CAMERA_RIGHT);
            velocity.x -= friction;
            if (velocity.x < 0.0)
                velocity.x = 0;
        }
        else {
            camera_move(selected_camera, CAMERA_LEFT);
            velocity.x += friction; 
            if (velocity.x > 0.0)
                velocity.x = 0;      
        }
        camera.move_mul.x = fabs(velocity.x);
        moved = 1;
    
    }
    if (velocity.z) {
        if (velocity.z > 0.0f) {
            camera_move(selected_camera, CAMERA_FORWARD);
            velocity.z -= friction;
            if (velocity.z < 0.0)
                velocity.z = 0;
        }
        else {
            camera_move(selected_camera, CAMERA_BACKWARD);
            velocity.z += friction;
            if (velocity.z > 0.0)
                velocity.z = 0;
        }
        camera.move_mul.z = fabs(velocity.z);
        moved = 1;
    }
    if ((camera.move_mul.x == max_velocity || camera.move_mul.x == -max_velocity) &&
        (camera.move_mul.z == max_velocity || camera.move_mul.z == -max_velocity))
    {
        camera.move_mul.x /= 2;
        camera.move_mul.z /= 2;
    }
    return moved;
}

int main() {
    init();
    velocity = (vector3f_t){0, 0, 0};
    handles_init();
    setup_handles();
    init_gl();
    handle_call(HANDLE_INIT, NULL);
    
    engine_setup_signals();
   
    SDL_Event event;
    time_init();
    
    shader_use(&shader_main);
    shader_use(&shader_depth);
    
    shadows_init(2048, 2048, light->direction, (vector3f_t){0, 0, 0});
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        shader_use(&shader_main);
        move();

        camera_update(selected_camera);
        shader_set_vec3f(&shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handle_call(HANDLE_DRAW, NULL);
        window_buffers_swap(&window);

        time_end();
        SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }

    return 0;
}

int on_end(void *arg){
    (void)arg;
    shader_destroy(&shader_depth);
    shader_destroy(&shader_main);
    window_destroy(&window);
    
    meshes_cleanup();
    textures_cleanup();
    objects_cleanup();
    
    log_msg(LOG_INFO, "Buffer usage at program end: %llu\n", buffer_total_used);
    
    SDL_Quit();
    exit(0);
}

void load_models() {
    stone = material_new((material_t){
        "Stone",
        texture_load(NULL, "../images/stone.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/stone_spec.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/stone_normal.bmp", IMAGE_BMP),
        0, 1, 2,
        true, 5.0f
    });
    
    brick = material_new((material_t){
        "Brick",
        texture_load(NULL, "../images/marble.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/marble_spec.bmp", IMAGE_BMP),
        NULL,
        0, 1, 2,
        false, 1.0f
    });
    
    wall = object_new();
    object_init("Wall", wall, 0);
    object_attach_mesh(wall, mesh_load("../models/conference.obj", MODEL_OBJ, 0));
    wall->position = (vector3f_t){0, 0, 3};
    wall->scale = (vector3f_t){0.01, 0.01, 0.01};
    
    level = object_new();
    object_init("Level", level, 0);
    object_attach_mesh(level, wall->mesh);
    object_update(level);
    level->position.z = -7;
    level->rotation.x = 1.57f;
    level->scale = (vector3f_t){2, 2, 1};
    
    box = object_new();
    object_init("Box", box, 0);
    object_attach_mesh(box, mesh_load("../models/cube.obj", MODEL_OBJ, 0));
    box->position = (vector3f_t){0, 3, 0};
    
    object_update(box);
    object_update(level);
    object_update(wall);

    light = light_new(LIGHT_DIRECTIONAL);
    light->direction = (vector3f_t){0.0,   5.0,   -5.0};
    light->ambient   = (vector3f_t){0.02f, 0.02f,  0.02f};
    light->diffuse   = (vector3f_t){0.15f, 0.15f,  0.15f};
    light->specular  = (vector3f_t){0.8f,  0.8f,   0.8f};
    light_init(light, &shader_main);
}

int render_scene(void *arg) {
    camera_t *cam = (camera_t *)arg;
    shader_use(&shader_main);
    material_update(brick, &shader_main);
    object_draw(wall, cam, &shader_main);
    material_update(stone, &shader_main);
    //object_draw(level, cam, &shader_main);
    object_draw(box, cam, &shader_main);
    return 0;
}
 
int on_draw(void *arg) {
    (void)arg;
    shadows_render(&shader_main, &shader_depth);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadow_fb.texture->id);
    shader_set_int(&shader_main, "shadow_map", 4);
    render_scene(selected_camera);
    
    return 0;
}

void check_mouse(double xrel, double yrel) {
    if (!(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED))
        return;
    selected_camera->rotation.x -= delta_time*(MOUSE_SPEED*xrel);
    selected_camera->rotation.y -= delta_time*(MOUSE_SPEED*yrel);
}

void check_event(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        game_info.flags &= ~GAME_IS_RUNNING;
    }
    else if (event->type == SDL_KEYDOWN) {
        controls_handle_keydown(event->key.keysym.sym);
    }
    else if (event->type == SDL_KEYUP) {
        controls_handle_keyup(event->key.keysym.sym);
    }
    /*else if (event->type == SDL_MOUSEBUTTONDOWN) {
        camera.fov = 25.0f;
        camera_select(&camera);
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        camera.fov = 75.0f;
        camera_select(&camera);
    }*/
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}
