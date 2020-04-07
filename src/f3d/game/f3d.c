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
void render_scene(shader_t *shader);
void end(int sig);

window_t window;
material_t *brick, *stone;
model_t wall, level, box;
float count = 0;
light_t *light, *light2;

camera_t camera, camera2;

unsigned shadow_fb;
unsigned shadow_tex;

unsigned arrayid;
shader_t shader_main;//, shader_depth;

int init(void) {
    game_init();
    game_info.flags |= GAME_IS_RUNNING;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    window = window_new("Ethan's 3D Engine", 1024, 800, 0);
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
    /*glGenFramebuffers(1, &shadow_fb);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fb);
    
    glGenTextures(1, &shadow_tex);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 800, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_tex, 0);
    glDrawBuffer(GL_NONE);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log_msg(LOG_ERROR, "Framebuffer incomplete\n", 0);
        return;
    }
    
    mat4_t projection, view, model;
    math_ortho(&projection, -10, 10, -10, 10, -10, 20);
    view = math_lookat(light->direction, (vector3f_t){0, 0, 0}, (vector3f_t){0, 1, 0});
    mat4_set(&model, MAT4_IDENTITY);
    
    mat4_t temp = mat4_mul(projection, view);
    mat4_t mvp  = mat4_mul(temp, model);
    
    mat4_t bias_mat;
    mat4_set(
        &bias_mat, 
        (float []){
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        }
    );*/
    //shader_set_mat4(&shader_depth, "depth_mvp", &mvp);
    //shader_set_mat4(&shader_depth, "depth_bias", &bias_mat);
}

void render_shadows() {
    //glBindFramebuffer(GL_FRAMEBUFFER, shadow_fb);
    //render_scene(&shader_depth);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
 
void init_gl() {
    glClearColor(0.0, 0.0, 0.0, 0);
    glViewport(0, 0, window.width, window.height);
    
    shader_main = shaders_link(
        "Main",
        shader_load("../shaders/pp/final_vert.glsl", SHADER_VERTEX),
        shader_load("../shaders/pp/final_frag.glsl", SHADER_FRAGMENT)
    );
    
    shader_use(&shader_main);

    camera = camera_new();
    camera.move_speed = 6.0f;
    camera.position = (vector3f_t){2, 3, -5};
    
    camera2 = camera_new();
    camera2.move_speed = 6.0f;
    camera2.position = (vector3f_t){-3, 3, -5};
    
    load_models();
    
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    
    //glEnable(GL_FRAMEBUFFER_SRGB);
    
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
    if (keys_pressed[CONTROL_1]) {
        camera_select(&camera);
    }
    else if (keys_pressed[CONTROL_2]) {
        camera_select(&camera2);
    }
    return moved;
}

int main() {    
    init();
    init_gl();
    signal(SIGINT, &end);
   
    SDL_Event event;
    time_init();
    
    init_shadows();
    shader_use(&shader_main);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);        
            
        shader_use(&shader_main);
        if (move()) {
            box.position = camera2.position;
            model_update(&box);
        }
        //camera.direction = (vector3f_t){0, 0, 0};
        camera_update(selected_camera);
        shader_set_vec3f(&shader_main, "view_pos", selected_camera->position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);

        time_end();
        //log_msg(LOG_INFO, "fps: %u\n", time_get_fps());
        SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    
    end(SIGINT);
    return 0;
}

void end(int sig) {
    (void)sig;
    shader_destroy(&shader_main);
    
    window_destroy(&window);
    
    meshes_cleanup();
    textures_cleanup();
    
    log_msg(LOG_INFO, "Buffer usage at program end: %llu\n", buffer_total_used);
    
    SDL_Quit();
}

void load_models() {
    stone = material_new((material_t){
        "Stone",
        texture_load("../images/stone.bmp", IMAGE_BMP, 0),
        texture_load("../images/stone_spec.bmp", IMAGE_BMP, 0),
        texture_load("../images/stone_normal.bmp", IMAGE_BMP, 0),
        0, 1, 2, 3.0f
    });
    
    brick = material_new((material_t){
        "Brick",
        texture_load("../images/brick.bmp", IMAGE_BMP, 0),
        texture_load("../images/brick_spec.bmp", IMAGE_BMP, 0),
        texture_load("../images/brick_normal.bmp", IMAGE_BMP, 0),
        0, 1, 2, 1.0f
    });
    
    wall.mesh = mesh_load("../models/wall.obj", MODEL_OBJ, 0);
    model_init("Wall", &wall, 0);
    wall.position = (vector3f_t){0, 0, 3};
    
    level.mesh = wall.mesh;
    model_init("Level", &level, 0);
    model_update(&level);
    level.position.z = -7;
    level.rotation.x = 1.57;
    
    box.mesh = mesh_load("../models/cube.obj", MODEL_OBJ, 0);
    model_init("Box", &box, 0);
    box.position = camera2.position;
    
    model_update(&box);
    model_update(&level);
    model_update(&wall);
    
    /*
    light = light_new(LIGHT_POINT);
    light->position = (vector3f_t){0.0f, 6.0f, -1.0f};
    light->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light->diffuse = (vector3f_t){0.8f, 0.8f, 0.8f};
    light->specular = (vector3f_t){1.0f, 1.0f, 1.0f};
    light->radius = 5.0f;
    light_init(light, &shader_main);

    light2 = light_new(LIGHT_POINT);
    light2->position = (vector3f_t){0.0f, 2.0f, -5.0f};
    light2->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light2->diffuse = (vector3f_t){0.8f, 0.8f, 0.8f};
    light2->specular = (vector3f_t){1.0f, 1.0f, 1.0f};
    light2->radius = 5.0f;
    light_init(light2, &shader_main);
    */
    light = light_new(LIGHT_DIRECTIONAL);
    light->direction = (vector3f_t){-2.0, 10.0, -10};
    light->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light->diffuse = (vector3f_t){0.15f, 0.15f, 0.15f};
    light->specular = (vector3f_t){0.8f, 0.8f, 0.8f};
    light->radius = 5.0f;
    light_init(light, &shader_main);
}

void render_scene(shader_t *shader) {
    shader_use(shader);
    material_update(brick, shader);
    model_draw(&wall, selected_camera, shader);
    material_update(stone, shader);
    model_draw(&level, selected_camera, shader);
    model_draw(&box, selected_camera, shader);
}
 
void draw() {
    render_scene(&shader_main);
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
