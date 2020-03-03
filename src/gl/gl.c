#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <gl/bmp.h>

#include <gl/window.h>
#include <gl/camera.h>
#include <gl/shader.h>
#include <gl/math.h>
#include <gl/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define MOUSE_SPEED 0.001
#define PLAYER_SPEED 0.10

camera_t camera;
window_t window;
bool running = true;
bool mouse_captured = true;

unsigned arrayid;
unsigned vbuf, cbuf;
unsigned progid;
 
void make_triangle() {
    static const float buffer_data[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
    unsigned textureid;
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);
    int width, height;
    BITMAPINFO *binfo;
    unsigned char *data = bmp_load("../images/test.bmp", &binfo);
    width = binfo->bmiHeader.biWidth;
    height = binfo->bmiHeader.biHeight;
    
    if (data == NULL) {
        printf("CANNOT LOAD IMAGE\n");
        exit(1);
    }
    else {
        printf("nice, %dx%d\n", binfo->bmiHeader.biWidth, binfo->bmiHeader.biHeight);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    static const GLfloat uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };
    glGenBuffers(1, &cbuf);
    glBindBuffer(GL_ARRAY_BUFFER, cbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
    
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
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        NULL                          // array buffer offset
    );
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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
 
void check_key(int key, uint8_t state) {
    (void)state;
    
    if (key == SDLK_w) {
        camera_move(&camera, CAMERA_DIRECTION_FORWARD, PLAYER_SPEED);
    }
    if (key == SDLK_s) {
        camera_move(&camera, CAMERA_DIRECTION_BACKWARD, PLAYER_SPEED);
    }
    if (key == SDLK_a) {
        camera_move(&camera, CAMERA_DIRECTION_LEFT, PLAYER_SPEED);
    }
    if (key == SDLK_d) {
        camera_move(&camera, CAMERA_DIRECTION_RIGHT, PLAYER_SPEED);
    }
    if (key == SDLK_ESCAPE) {
        mouse_captured = !mouse_captured;
        SDL_SetRelativeMouseMode(mouse_captured);
    }
}

void check_event(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        check_key(event->key.keysym.sym, event->key.state);
    }
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
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
            
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(progid);
        camera_update(&camera, progid);
        draw();
        window_buffers_swap(&window);
    }
    window_destroy(&window);
    SDL_Quit();
    return 0;
}
