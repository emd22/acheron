#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <signal.h>

#include <f3d/engine/engine.h>

#include <f3d/game/game.h>

#include <GL/glew.h>
#include <GL/gl.h>


#define FRAMERATE_CAP 60

#define MOUSE_SPEED 0.07

void load_models();
void draw();
void check_mouse(double xrel, double yrel);
void check_event(SDL_Event *event);
void render_scene(shader_t *shader, camera_t *rcamera);
void end(int sig);

window_t window;
material_t *brick, *stone;
//model_t wall, level, box;
object_t *wall, *level, *box, *player;
float count = 0;
light_t *light, *light2;

mat4_t bias_mat;

camera_t camera, shadow_cam;

unsigned shadow_fb;
unsigned shadow_tex;

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
    
    glGenVertexArrays(1, &arrayid);
    glBindVertexArray(arrayid);
    return 0;
}

// http://www.it.hiof.no/~borres/j3d/explain/shadow/p-shadow.html

void init_shadows() {
    glGenFramebuffers(1, &shadow_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fb);
    
    glGenTextures(1, &shadow_tex);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 700, 700, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0);
    glDrawBuffer(GL_NONE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    math_ortho(&shadow_cam.mat_projection, -10, 10, -10, 10, -10, 20);
    shadow_cam.mat_view = math_lookat(light->direction, (vector3f_t){0.0, 0, 0}, (vector3f_t){0, 1, 0});
}

void render_shadows() {
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fb);
    mat4_t model;
    mat4_set(&model, MAT4_IDENTITY);
    
    shadow_cam.mat_view = math_lookat(light->direction, (vector3f_t){0.0, 0, 0}, (vector3f_t){0, 1, 0});
    mat4_t temp   = mat4_mul(shadow_cam.mat_projection, shadow_cam.mat_view);
    mat4_t mvp    = mat4_mul(bias_mat, temp);
    shader_set_mat4(&shader_main, "shadow_bias", &mvp);
    shader_use(&shader_depth);
    shader_set_mat4(&shader_depth, "depth_mvp", &mvp);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    render_scene(&shader_main, &shadow_cam);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader_use(&shader_main);
}
 
void init_gl() {
    glClearColor(0.0, 0.1, 0.0, 1);
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

int move() {
    int moved = 0;
    if (keys_pressed[CONTROL_FORWARD]) {
        camera_move(selected_camera, CAMERA_FORWARD);
        moved = 1;
    }
    if (keys_pressed[CONTROL_BACKWARD]) {
        camera_move(selected_camera, CAMERA_BACKWARD);
        moved = 1;
    }
    if (keys_pressed[CONTROL_LEFT]) {
        camera_move(selected_camera, CAMERA_LEFT);
        moved = 1;
    }
    if (keys_pressed[CONTROL_RIGHT]) {
        camera_move(selected_camera, CAMERA_RIGHT);
        moved = 1;
    }
    /*if (keys_pressed[CONTROL_1]) {
        box->position.y = 20;
        box->physics.velocity = 0;
        box->physics.locked = false;
        player->physics.position = camera.position;
        object_update(box);
    }*/
    return moved;
}

int main() {    
    init();
    init_gl();
    signal(SIGINT, &end);
   
    SDL_Event event;
    time_init();
    
    shader_use(&shader_depth);
    init_shadows();
    shader_use(&shader_main);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    mat4_set(
        &bias_mat, 
        (float []){
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        }
    );
    
    float up = 0;
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        up++;
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);        
            
        shader_use(&shader_main);
        move();
        box->position.y = sin(up*0.02)+3;
        object_update(box);
        //physics_update(&(box->physics));
        //box->position = box->physics.position;
        //object_update(box);
        //if (object_check_collision(box, level)) {
            //box->physics.locked = true;
            //box->physics.velocity = -(box->physics.velocity*0.97);
        //}
        camera_update(selected_camera);
        shader_set_vec3f(&shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);

        time_end();
        SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    
    end(SIGINT);
    return 0;
}

void end(int sig) {
    (void)sig;
    shader_destroy(&shader_depth);
    shader_destroy(&shader_main);
    window_destroy(&window);
    
    meshes_cleanup();
    textures_cleanup();
    objects_cleanup();
    
    log_msg(LOG_INFO, "Buffer usage at program end: %llu\n", buffer_total_used);
    
    SDL_Quit();
}

void load_models() {
    stone = material_new((material_t){
        "Stone",
        texture_load("../images/stone.bmp", IMAGE_BMP, 0),
        texture_load("../images/stone_spec.bmp", IMAGE_BMP, 0),
        texture_load("../images/stone_normal.bmp", IMAGE_BMP, 0),
        0, 1, 2, 5.0f
    });
    
    brick = material_new((material_t){
        "Brick",
        texture_load("../images/brick.bmp", IMAGE_BMP, 0),
        texture_load("../images/brick_spec.bmp", IMAGE_BMP, 0),
        texture_load("../images/brick_normal.bmp", IMAGE_BMP, 0),
        0, 1, 2, 1.0f
    });
    
    wall = object_new();
    object_init("Wall", wall, 0);
    object_attach_mesh(wall, mesh_load("../models/wall.obj", MODEL_OBJ, 0));
    wall->position = (vector3f_t){0, 0, 3};
    
    level = object_new();
    object_init("Level", level, 0);
    object_attach_mesh(level, wall->mesh);
    object_update(level);
    level->position.z = -7;
    level->rotation.x = 1.57f;
    
    box = object_new();
    object_init("Box", box, 0);
    object_attach_mesh(box, mesh_load("../models/cube.obj", MODEL_OBJ, 0));
    box->position = (vector3f_t){0, 3, 0};
    
    object_update(box);
    object_update(level);
    object_update(wall);

    light = light_new(LIGHT_DIRECTIONAL);
    light->direction = (vector3f_t){2.0,   10.0,   -10.0};
    light->ambient   = (vector3f_t){0.02f, 0.02f,  0.02f};
    light->diffuse   = (vector3f_t){0.15f, 0.15f,  0.15f};
    light->specular  = (vector3f_t){0.8f,  0.8f,   0.8f};
    light_init(light, &shader_main);
}

void render_scene(shader_t *shader, camera_t *cam) {
    shader_use(shader);
    material_update(brick, shader);
    object_draw(wall, cam, shader);
    material_update(stone, shader);
    object_draw(level, cam, shader);
    object_draw(box,   cam, shader);
}
 
void draw() {
    render_shadows();
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
    shader_set_int(&shader_main, "shadow_map", 6);
    render_scene(&shader_main, selected_camera);
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
