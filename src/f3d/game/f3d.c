#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <f3d/engine/engine.h>

#include <f3d/game/game.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define FRAMERATE_CAP 60

#define MOUSE_SPEED 0.07

void set_material();
void load_models();
void draw_models();
void draw();
void check_mouse(double xrel, double yrel);
void check_event(SDL_Event *event);

window_t window;
material_t *stone, *marble;
model_t cube, level;
float count = 0;
light_t *light;

camera_t camera;

unsigned arrayid;
unsigned progid;

int init(void) {
    game_init();
    game_info.flags |= GAME_IS_RUNNING;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    window = window_new("Ethan's 3D Engine", 1024, 800, 0);
    default_window = &window;
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_msg(LOG_FATAL, "Could not initialize GLEW\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(0);
    glGenVertexArrays(1, &arrayid);
    glBindVertexArray(arrayid);
    return 0;
}
 
void init_gl() {
    glClearColor(0.0, 0.0, 0.0, 0);
    glViewport(0, 0, window.width, window.height);
    
    unsigned vert = shader_load("../shaders/m_vert.glsl", SHADER_VERTEX);
    unsigned frag = shader_load("../shaders/m_frag.glsl", SHADER_FRAGMENT);
    progid = shaders_link(vert, frag);
    glUseProgram(progid);
    
    camera = camera_new();
    camera.move_speed = 7.0f;
    camera.position = (vector3f_t){0, 3, 0};
    camera.rotation = (vector3f_t){0, 0, 0};
    
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    
    load_models();
    set_material();
    
    //glEnable(GL_FRAMEBUFFER_SRGB);
    
    // fix overlapping polygons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
}

void move() {
    if (keys_pressed[CONTROL_FORWARD])
        camera_move(&camera, CAMERA_FORWARD);
    if (keys_pressed[CONTROL_BACKWARD])
        camera_move(&camera, CAMERA_BACKWARD);
    if (keys_pressed[CONTROL_LEFT])
        camera_move(&camera, CAMERA_LEFT);
    if (keys_pressed[CONTROL_RIGHT])
        camera_move(&camera, CAMERA_RIGHT);
    //flashlight->position = camera.position;
    //light_update(flashlight, progid);
}

int main() {    
    init();
    init_gl();
   
    SDL_Event event;
    time_init();
   
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);        
            
        glUseProgram(progid);
        move();
        //camera.direction = (vector3f_t){0, 0, 0};
        camera_update(&camera, progid);
        shader_set_vec3f(progid, "viewPos", camera.position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);

        time_end();
        //log_msg(LOG_INFO, "fps: %u\n", time_get_fps());
        SDL_Delay((1000/FRAMERATE_CAP-delta_time));
    }
    glDeleteProgram(progid);
    material_destroy(stone);
    material_destroy(marble);
    model_destroy(&cube);
    model_destroy(&level);
    window_destroy(&window);
    
    SDL_Quit();
    return 0;
}

void load_models() {
    marble = material_new((material_t){
        "Marble",
        texture_load("../images/stone.bmp", IMAGE_BMP),
        texture_load("../images/marble_spec.bmp", IMAGE_BMP),
        NULL,
        0, 1, 2, 32.0f
    });
    
    stone = material_new((material_t){
        "Stone",
        texture_load("../images/marble.bmp", IMAGE_BMP),
        texture_load("../images/stone_spec.bmp", IMAGE_BMP),
        texture_load("../images/normal.bmp", IMAGE_BMP),
        0, 1, 2, 32.0f
    });

    cube = model_load("Cube", "../models/cube.obj", MODEL_OBJ);
    level = model_load("Level", "../models/level.obj", MODEL_OBJ);
    cube.position = (vector3f_t){0, 3, 3};
}

void set_material(void) {
    light = light_new(LIGHT_POINT);
    light->position = (vector3f_t){0.0f, 3.0f, 0.0f};
    light->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light->diffuse = (vector3f_t){0.8f, 0.8f, 0.8f};
    light->specular = (vector3f_t){1.0f, 1.0f, 1.0f};
    light->constant = 1.0f;
    light->linear = 0.09f;
    light->quadratic = 0.032f;
    light_init(light, progid);
}

void draw_models() {
    //cube.rotation.x += 0.6*delta_time;
    //cube.rotation.z += 0.4*delta_time;
    
    material_update(stone, progid);
    model_draw(&cube, &camera, progid);
    //material_update(marble, progid);
    model_draw(&level, &camera, progid);
}
 
void draw() {
    draw_models();
}

void check_mouse(double xrel, double yrel) {
    if (!(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED))
        return;
    camera.rotation.x -= MOUSE_SPEED*delta_time*xrel;
    camera.rotation.y -= MOUSE_SPEED*delta_time*yrel;
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
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        camera.fov = 45.0f;
        camera_select(&camera);
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        camera.fov = 75.0f;
        camera_select(&camera);
    }
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}
