#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <gl/texture.h>
#include <gl/window.h>
#include <gl/camera.h>
#include <gl/shader.h>
#include <gl/math.h>
#include <gl/log.h>
#include <gl/model.h>
#include <gl/time.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define MOUSE_SPEED 0.001
#define PLAYER_SPEED 0.10

camera_t camera;
window_t window;
texture_t image, image_spec;
model_t model, model2;
bool running = true;
bool mouse_captured = true;

unsigned arrayid;
unsigned progid;
 
void load_models() {
    
    image = texture_load("../images/brick.bmp");
    image_spec = texture_load("../images/brick_spec.bmp");
    
    // assign image to texture0 in fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image.id);
    // assign specular to texture1 in fragment shader
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, image_spec.id);

    model = model_load("../models/cube.obj", MODEL_OBJ);
    model2 = model_load("../models/level.obj", MODEL_OBJ);
    mat4_translate(&(model2.matrix), (vector3f_t){0, -2, 0});
}

void set_material(void) {
    shader_set_int(progid, "material.diffuse", 0);
    shader_set_int(progid, "material.specular", 1);
    shader_set_float(progid, "material.shininess", 32.0f);
    
    shader_set_vec3f(progid, "dirLight.direction", (vector3f_t){-1.0f, -2.0f, -0.40f});
    shader_set_vec3f(progid, "dirLight.ambient", (vector3f_t){0.15f, 0.15f, 0.15f});
    shader_set_vec3f(progid, "dirLight.diffuse", (vector3f_t){0.4f, 0.4f, 0.4f});
    shader_set_vec3f(progid, "dirLight.specular", (vector3f_t){0.5f, 0.5f, 0.5f});
}

void draw_models() {
    model.matrix = mat4_rotate_z(model.matrix, 0.01);
    model_draw(&model, &camera, progid);
    model_draw(&model2, &camera, progid);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glViewport(0, 0, window.width, window.height);
    
    set_material();
    
    camera = camera_new();
    camera.move_speed = 0.1f;
    camera.strafe_speed = 0.1f;
    camera.position.z = -5;
    
    // select camera to be default and calculate perspective matrix
    camera_select(&camera);
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    
    load_models();
    
    // fix overlapping polygons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
}
 
void draw() {
    draw_models();
}

void check_mouse(double xrel, double yrel) {
    if (!mouse_captured)
        return;
    const float rotx = (xrel*MOUSE_SPEED);
    const float roty = (yrel*MOUSE_SPEED);
    camera.rotation.y += rotx;
    camera.rotation.x += roty;
}
 
void on_keydown(int key) {
    if (key == SDLK_w) {
        camera.move.z = 1.0f;
    }
    else if (key == SDLK_s) {
        camera.move.z = -1.0f;
    }
    else if (key == SDLK_a) {
        camera.move.x = -1.0f;
    }
    else if (key == SDLK_d) {
        camera.move.x = 1.0f;
    }
    else if (key == SDLK_ESCAPE) {
        mouse_captured = !mouse_captured;
        SDL_SetRelativeMouseMode(mouse_captured);
    }
    else if (key == SDLK_q) {
        running = false;
    }
}

void on_keyup(int key) {
    if (key == SDLK_w) {
        camera.move.z = 0.0f;
    }
    else if (key == SDLK_s) {
        camera.move.z = 0.0f;
    }
    else if (key == SDLK_a) {
        camera.move.x = 0.0f;
    }
    else if (key == SDLK_d) {
        camera.move.x = 0.0f;
    }
}

void check_event(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        on_keydown(event->key.keysym.sym);
    }
    else if (event->type == SDL_KEYUP) {
        on_keyup(event->key.keysym.sym);
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
 
int main() {    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n", 0);
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    window = window_new("Ethan's 3D Engine", 800, 600, 0);
    window_set_default(&window);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_msg(LOG_FATAL, "Could not initialize GLEW\n", 0);
        return 1;
    }
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(1);
    glGenVertexArrays(1, &arrayid);
    glBindVertexArray(arrayid);
    
    unsigned vert = shader_load("../shaders/m_vert.glsl", SHADER_VERTEX);
    unsigned frag = shader_load("../shaders/m_frag.glsl", SHADER_FRAGMENT);
    progid = shaders_link(vert, frag);
    glUseProgram(progid);

    init();
    
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
   
    SDL_Event event;
    
    time_init();
   
    while (running) {
        while (SDL_PollEvent(&event))
            check_event(&event);
        
        time_tick();
            
        glUseProgram(progid);
        if (camera.move.x || camera.move.z)
            camera_move(&camera);
        camera_update(&camera, progid);
        shader_set_vec3f(progid, "viewPos", camera.position);
        //unsigned lightid = glGetUniformLocation(progid, "lightPos");
        //float lpos[3] = {2, 2, 2};
        //glUniform3fv(lightid, 3, lpos);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);

        //log_msg(LOG_INFO, "fps: %u\n", time_get_fps());
    }
    glDeleteProgram(progid);
    texture_destroy(&image);
    texture_destroy(&image_spec);
    model_destroy(&model);
    model_destroy(&model2);
    window_destroy(&window);
    
    SDL_Quit();
    return 0;
}
