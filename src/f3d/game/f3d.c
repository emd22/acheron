#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

#include <f3d/engine/engine.h>
#include <f3d/engine/rendering/render.h>

#include <f3d/game/game.h>

#include <GL/glew.h>
#include <GL/gl.h>


#define FRAMERATE_CAP 60

#define MOUSE_SPEED 0.07

void load_models();
int on_draw(void *arg);
void check_mouse(double xrel, double yrel);
void check_event(SDL_Event *event);
int on_end(void *arg);

window_t window;
material_t *brick, *stone;
light_t *light;
camera_t camera;

scene_t *scene;

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
    
    return 0;
}
 
void init_gl() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glViewport(0, 0, window.width, window.height);
    
    meshes_init();
    render_init();
    
    shader_use(shader_main);

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
}

int move() {
    int moved = 0;
    
    static vector3f_t velocity;
    const float max_velocity = 1.0f;
    
    if (keys[CONTROL_FORWARD].pressed) {
        velocity.z = max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_BACKWARD].pressed) {
        velocity.z = -max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_LEFT].pressed) {
        velocity.x = -max_velocity;
        moved = 1;
    }
    if (keys[CONTROL_RIGHT].pressed) {
        velocity.x = max_velocity;
        moved = 1;
    }
    if (controls_check_toggle(CONTROL_1)) {
        stone->flags ^= MATERIAL_NO_NORMALMAP;
        material_update(stone, shader_main);
    }
    if (controls_check_toggle(CONTROL_2)) {
        static bool wireframe = false;
        wireframe = !wireframe;
        engine_render_wireframe(wireframe);

    }
    const float friction = 0.03;
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
    // if we move two directions at the same time, half the speed
    // so we don't move faster than normal.
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
    handles_init();
    setup_handles();
    init_gl();
    handle_call(HANDLE_INIT, NULL);
    shadows_init(2048, 2048, light->direction, (vector3f_t){0, 0, 0});
    
    scene = scene_new("Scene");
    scene_new_view(scene, &camera, 100, 100, GL_COLOR_ATTACHMENT0);
    render_view_t *view2;
    view2 = scene_new_view(scene, &shadow_cam, 100, 100, GL_COLOR_ATTACHMENT0);
    view2->dest_position.x = 100;
    
    engine_setup_signals();
   
    SDL_Event event;
    time_init();
    
    shader_use(shader_main);
    shader_use(shader_depth);
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        shader_use(shader_main);
        move();
        
        camera_update(selected_camera);
        shader_set_vec3f(shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //render_all();
        handle_call(HANDLE_DRAW, NULL);
        window_buffers_swap(&window);

        time_end();
        //SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    handle_call(HANDLE_END, NULL);
    return 0;
}

int on_end(void *arg){
    (void)arg;
    //shader_destroy(shader_depth);
    //shader_destroy(shader_main);
    window_destroy(&window);
    
    meshes_cleanup();
    textures_cleanup();
    //objects_cleanup();
    
    log_msg(LOG_INFO, "Buffer usage at program end: %llu\n", buffer_total_used);
    //os_print_backtrace();
    
    SDL_Quit();
    exit(0);
}

void load_models() {
    stone = material_new((material_t){
        "Stone", 0,
        texture_load(NULL, "../images/stone.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/stone_spec.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/stone_normal.bmp", IMAGE_BMP),
        32.0f, 0
    });
    brick = material_new((material_t){
        "Brick", 0,
        texture_load(NULL, "../images/brick.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/brick_spec.bmp", IMAGE_BMP),
        texture_load(NULL, "../images/brick_normal.bmp", IMAGE_BMP),
        32.0f, 0
    });

    render_object_t *level = object_new("Level");
    object_attach(level, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/wall.obj", MODEL_OBJ, 0));
    object_attach(level, OBJECT_ATTACH_MATERIAL, brick);
    object_rotate(level, 1.57, 0.0f, 0.0f);
    object_move(level, 0, 0, -5);

    render_object_t *wall = object_new("Wall");
    object_attach(wall, OBJECT_ATTACH_MESH, level->mesh);
    object_attach(wall, OBJECT_ATTACH_MATERIAL, stone);

    render_object_t *box = object_new("Box");
    object_attach(box, OBJECT_ATTACH_MESH, mesh_load(NULL, "../models/cube.obj", MODEL_OBJ, 0));
    object_attach(box, OBJECT_ATTACH_MATERIAL, stone);
    object_move(box, 0, 2, 0);
    
    light = light_new(LIGHT_DIRECTIONAL);
    light->direction = (vector3f_t){-0.2, 0.8, -0.7};
    light->ambient   = (vector3f_t){0.02f, 0.02f,  0.02f};
    light->diffuse   = (vector3f_t){0.15f, 0.15f,  0.15f};
    light->specular  = (vector3f_t){0.8f,  0.8f,   0.8f};
    light_init(light, shader_main);
    
    objects_sort();
    default_framebuffer = NULL;
}
 
int on_draw(void *arg) {
    (void)arg;
    
    render_all();
    
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
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}
