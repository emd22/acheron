#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <gl/window.h>
#include <gl/camera.h>
#include <gl/shader.h>
#include <gl/math.h>
#include <gl/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <math.h>

#define MOUSE_SPEED 0.10
#define PLAYER_SPEED 0.10

camera_t camera;
window_t window;
bool running = true;
bool mouse_captured = true;

unsigned arrayid;
unsigned vbuf;
unsigned progid;
 
void make_triangle() {
    static const float buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };
    glGenBuffers(1, &vbuf);
    glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void init() {
    glClearColor(0.2, 0.0, 0.2, 1);
    glViewport(0, 0, 1024, 768);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    log_msg(LOG_INFO, "OpenGL Camera initialized\n", 0);
    camera = camera_new();
    camera.position.z = -7;
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
    const float roty = math_deg_to_rad(xrel*MOUSE_SPEED);
    const float rotx = math_deg_to_rad(yrel*MOUSE_SPEED);
    camera.rotation.x += rotx;
    camera.rotation.y += roty;
    //camera.direction.x = cos(roty)*sin(rotx);
    //camera.direction.y += sin(roty);
    //camera.direction.z = cos(roty)*sin(rotx);
    camera_clamp_rotation(&camera);
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
    else if (key == SDLK_ESCAPE) {
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
    
    window = window_new("Ethan's 3D Engine", 1024, 768, 0);
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
    
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
   
    SDL_Event event;
   
    while (running) {
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(progid);
        camera_update(&camera, progid);
        draw();
        window_buffers_swap(&window);
    }
    window_destroy(&window);
    SDL_Quit();
    return 0;
}
