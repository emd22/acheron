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
void end(int sig);

window_t window;
material_t *brick, *stone;
model_t wall, level, box;
float count = 0;
light_t *light, *light2;

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
    
    load_models();

    camera = camera_new();
    camera.move_speed = 6.0f;
    camera.position = (vector3f_t){0, 3, -5};
    
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
    signal(SIGINT, &end);
   
    SDL_Event event;
    time_init();
   
    log_msg(LOG_INFO, "Buffer usage: %.01fKB\n", (double)buffer_total_used/1024.0);
    while (game_info.flags & GAME_IS_RUNNING) {
        time_tick();
        while (SDL_PollEvent(&event))
            check_event(&event);        
            
        glUseProgram(progid);
        move();
        //camera.direction = (vector3f_t){0, 0, 0};
        camera_update(&camera, progid);
        shader_set_vec3f(progid, "view_pos", camera.position);
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
    glDeleteProgram(progid);
    
    window_destroy(&window);
    
    meshes_cleanup();
    textures_cleanup();
    
    log_msg(LOG_INFO, "Buffer usage at program end: %.01fKB\n", (double)buffer_total_used/1024.0);
    
    SDL_Quit();
}

void load_models() {
    stone = material_new((material_t){
        "Stone",
        texture_load("../images/stone.bmp", IMAGE_BMP),
        texture_load("../images/stone_spec.bmp", IMAGE_BMP),
        texture_load("../images/stone_normal.bmp", IMAGE_BMP),
        0, 1, 2, 3.0f
    });
    
    brick = material_new((material_t){
        "Brick",
        texture_load("../images/brick.bmp", IMAGE_BMP),
        texture_load("../images/brick_spec.bmp", IMAGE_BMP),
        texture_load("../images/ceiling.bmp", IMAGE_BMP),
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
    box.position.y = 3;
    
    
    light = light_new(LIGHT_POINT);
    light->position = (vector3f_t){0.0f, 6.0f, -1.0f};
    light->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light->diffuse = (vector3f_t){0.8f, 0.8f, 0.8f};
    light->specular = (vector3f_t){1.0f, 1.0f, 1.0f};
    light->radius = 5.0f;
    light_init(light, progid);

    light2 = light_new(LIGHT_POINT);
    light2->position = (vector3f_t){0.0f, 2.0f, -5.0f};
    light2->ambient = (vector3f_t){0.02f, 0.02f, 0.02f};
    light2->diffuse = (vector3f_t){0.8f, 0.8f, 0.8f};
    light2->specular = (vector3f_t){1.0f, 1.0f, 1.0f};
    light2->radius = 5.0f;
    light_init(light2, progid);
}

 
void draw() {
    material_update(brick, progid);
    model_draw(&wall, &camera, progid);
    material_update(stone, progid);
    model_draw(&level, &camera, progid);
    model_draw(&box, &camera, progid);
}

void check_mouse(double xrel, double yrel) {
    if (!(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED))
        return;
    camera.rotation.x -= delta_time*(MOUSE_SPEED*xrel);
    camera.rotation.y -= delta_time*(MOUSE_SPEED*yrel);
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
        camera.fov = 25.0f;
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
