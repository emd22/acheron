#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <f3d/engine/engine.h>

#include <f3d/game/game.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define MOUSE_SPEED 0.001
#define PLAYER_SPEED 0.10

void set_material();
void load_models();
void draw_models();
void draw();
void check_mouse(double xrel, double yrel);
void on_keydown(int key);
void on_keyup(int key);
void check_event(SDL_Event *event);

window_t window;
material_t *brick, *level;
model_t model, model2;

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
    
    window = window_new("Ethan's 3D Engine", 800, 600, 0);
    default_window = &window;
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_msg(LOG_FATAL, "Could not initialize GLEW\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(1);
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
    camera.move_speed = 0.1f;
    camera.strafe_speed = 0.1f;
    camera.position = (vector3f_t){0, 0, 0};
    
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    
    load_models();
    set_material();
    
    // fix overlapping polygons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //glEnable(GL_CULL_FACE);
    
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
}

int main() {    
    init();
    init_gl();
   
    SDL_Event event;
    
    time_init();
   
    while (game_info.flags & GAME_IS_RUNNING) {
        while (SDL_PollEvent(&event))
            check_event(&event);
        
        time_tick();
            
        glUseProgram(progid);
        move();
        //camera.direction = (vector3f_t){0, 0, 0};
        camera_update(&camera, progid);
        shader_set_vec3f(progid, "viewPos", camera.position);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);

        //log_msg(LOG_INFO, "fps: %u\n", time_get_fps());
    }
    glDeleteProgram(progid);
    material_destroy(level);
    material_destroy(brick);
    model_destroy(&model);
    model_destroy(&model2);
    window_destroy(&window);
    
    SDL_Quit();
    return 0;
}

void load_models() {
    brick = material_new((material_t){
        "Brick",
        texture_load("../images/brick.bmp", IMAGE_BMP),
        texture_load("../images/brick_spec.bmp", IMAGE_BMP),
        0, 1, 22.0f
    });
    
    level = material_new((material_t){
        "Level",
        texture_load("../images/bama.bmp", IMAGE_BMP),
        texture_load("../images/bama_spec.bmp", IMAGE_BMP),
        0, 1, 2.0f
    });

    model = model_load("Cube", "../models/cube.obj", MODEL_OBJ);
    model2 = model_load("Level", "../models/level.obj", MODEL_OBJ);
    mat4_translate(&(model.matrix), (vector3f_t){0, 0, -3});
    mat4_translate(&(model2.matrix), (vector3f_t){0, -2, 0});
}

void set_material(void) {
    
    shader_set_vec3f(progid, "dirLight.direction", (vector3f_t){-1.0f, -2.0f, -0.40f});
    shader_set_vec3f(progid, "dirLight.ambient", (vector3f_t){0.15f, 0.15f, 0.15f});
    shader_set_vec3f(progid, "dirLight.diffuse", (vector3f_t){0.4f, 0.4f, 0.4f});
    shader_set_vec3f(progid, "dirLight.specular", (vector3f_t){0.5f, 0.5f, 0.5f});
}

void draw_models() {
    model.matrix = mat4_rotate_z(model.matrix, 0.01);
    model.matrix = mat4_rotate_x(model.matrix, 0.02);
    material_update(brick, progid);
    model_draw(&model, &camera, progid);
    material_update(level, progid);
    model_draw(&model2, &camera, progid);
}
 
void draw() {
    draw_models();
}

void check_mouse(double xrel, double yrel) {
    if (!(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED))
        return;
    const float rotx = (xrel*MOUSE_SPEED);
    const float roty = (yrel*MOUSE_SPEED);
    camera.rotation.x += rotx;
    camera.rotation.y += roty;
    //log_msg(LOG_INFO, "%f, %f\n", math_rad_to_deg(camera.rotation.x), math_rad_to_deg(camera.rotation.y));
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
        camera.fov = 95.0f;
        camera_select(&camera);
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        camera.fov = 45.0f;
        camera_select(&camera);
    }*/
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}
