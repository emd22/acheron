#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <gl/window.h>
#include <gl/camera.h>
#include <gl/log.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>

#define MOUSE_SPEED 0.10
#define PLAYER_SPEED 0.25

camera_t camera;
window_t window;
bool running = true;
bool mouse_captured = true;
 
void init() {
    glClearColor(0.0, 0, 0.0, 1);
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    log_msg(LOG_INFO, "OpenGL Camera initialized\n");
    camera = camera_new();
    camera_select(&camera);
    //camera.pitch = 10;
    log_msg(LOG_INFO, "Camera initialized\n");
}
 
void draw() {
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0,  2.0, -5.0);
        glVertex3f(-2.0, -2.0, -5.0);
        glVertex3f( 2.0, -2.0, -5.0);
    glEnd();
    camera_update(&camera);
}

void check_mouse(double xrel, double yrel) {
    if (!mouse_captured)
        return;
    camera.pitch += yrel*MOUSE_SPEED;
    camera.yaw   += xrel*MOUSE_SPEED;
    camera_check(&camera);
    log_msg(LOG_INFO, "RY:%f RP:%f\n", camera.yaw, camera.pitch);
}
 
void check_key(int key, uint8_t state) {
    (void)state;
    
    if (key == SDLK_w) {
        camera_move(&camera, 1, PLAYER_SPEED);
    }
    if (key == SDLK_s) {
        camera_move(&camera, -1, PLAYER_SPEED);
    }
    if (key == SDLK_a) {
        camera_move(&camera, 2, PLAYER_SPEED);
    }
    if (key == SDLK_d) {
        camera_move(&camera, 3, PLAYER_SPEED);
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
        check_mouse(-event->motion.xrel, -event->motion.yrel);
    }
}
 
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_msg(LOG_FATAL, "Could not start SDL\n");
        return 1;
    }

    window = window_new("Ethan's 3D Engine", 640, 480, 0);
    window_set_default(&window);
    SDL_GL_SetSwapInterval(0);
    
    init();
   
    window_set_mouse_mode(WINDOW_MOUSE_DISABLED);
   
    SDL_Event event;
   
    while (running) {
        while (SDL_PollEvent(&event))
            check_event(&event);
            
        glClear(GL_COLOR_BUFFER_BIT);
        draw();
        window_buffers_swap(&window);
        //glfwSwapBuffers(window);
        //glfwPollEvents();
    }
    window_destroy(&window);
    SDL_Quit();
    return 0;
}
