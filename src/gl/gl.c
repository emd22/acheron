#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <gl/image.h>

#include <gl/window.h>
#include <gl/camera.h>
#include <gl/shader.h>
#include <gl/math.h>
#include <gl/log.h>

#include <gl/model/obj.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define MOUSE_SPEED 0.001
#define PLAYER_SPEED 0.10

camera_t camera;
window_t window;
image_t image;
model_obj_t model;
bool running = true;
bool mouse_captured = true;

unsigned arrayid;
unsigned vbuf, cbuf, nbuf;
unsigned progid;
 
void make_triangle() {
    model = obj_load("../models/test.obj");

    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.index*sizeof(vector3f_t), model.vertices.data, GL_STATIC_DRAW);
    unsigned textureid;
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);
    
    image = image_load("../images/bama.bmp", IMAGE_BMP);
    
    if (image.data == NULL) {
        printf("Error loading image\n");
        exit(1);
    }
    log_msg(LOG_INFO, "Loaded image of size %dx%d\n", image.width, image.height);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenBuffers(1, &cbuf);
    glBindBuffer(GL_ARRAY_BUFFER, cbuf);
    glBufferData(GL_ARRAY_BUFFER, model.uvs.index*sizeof(vector2f_t), model.uvs.data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &nbuf);
    glBindBuffer(GL_ARRAY_BUFFER, nbuf);
    glBufferData(GL_ARRAY_BUFFER, model.normals.index*sizeof(vector3f_t), model.normals.data, GL_STATIC_DRAW);
    
}

void draw_triangle() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glVertexAttribPointer(
        0, 3,
        GL_FLOAT,
        GL_FALSE,
        0, NULL
    );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cbuf);
    glVertexAttribPointer(
        1, 2, 
        GL_FLOAT, 
        GL_FALSE, 
        0,
        NULL
    );
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, nbuf);
    glVertexAttribPointer(
        2, 3,
        GL_FLOAT,
        GL_FALSE,
        0,
        NULL
    );
    
    glDrawArrays(GL_TRIANGLES, 0, model.vertices.size*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void init() {
    glClearColor(0.2, 0.0, 0.2, 1);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    log_msg(LOG_INFO, "OpenGL Camera initialized\n", 0);
    camera = camera_new();
    camera.move_speed = 0.1f;
    camera.strafe_speed = 0.1f;
    //camera.position.z = -4;
    camera_select(&camera);
    //camera.pitch = 10;
    log_msg(LOG_INFO, "Camera initialized\n", 0);
    make_triangle();
}
 
void draw() {
    draw_triangle();
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

    init();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
   
    SDL_Event event;
   
    while (running) {
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        glUseProgram(progid);
        if (camera.move.x || camera.move.z)
            camera_move(&camera);
        camera_update(&camera, progid);
        //unsigned lightid = glGetUniformLocation(progid, "lightPos");
        //float lpos[3] = {2, 2, 2};
        //glUniform3fv(lightid, 3, lpos);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);
    }
    glDeleteProgram(progid);
    image_destroy(&image);
    obj_destroy(&model);
    window_destroy(&window);
    
    SDL_Quit();
    return 0;
}
