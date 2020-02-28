#include <gl/window.h>
#include <gl/log.h>

#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

window_t *default_window = NULL;

window_t window_new(const char *title, int width, int height, int flags) {
    (void)flags;
    window_t window;
    log_msg(LOG_INFO, "Creating window '%s' with size %dx%d\n", title, width, height);
    window.win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    if (window.win == NULL) {
        log_msg(LOG_ERROR, "Could not create SDL Window. Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    window.width = width;
    window.height = height;
    window.context = SDL_GL_CreateContext(window.win);
    if (window.context == NULL) {
        log_msg(LOG_ERROR, "window.context == NULL!\n", 0);
    }
    window.flags   = WINDOW_INITED;
    return window;
}

void window_set_default(window_t *window) {
    log_msg(LOG_INFO, "Setting window with size %dx%d as default\n", window->width, window->height);
    default_window = window;
}

window_t *window_get_default(void) {
    log_msg(LOG_INFO, "Getting window with size %dx%d\n", default_window->width, default_window->height);
    return default_window;
}

void window_buffers_swap(window_t *window) {
    SDL_GL_SwapWindow(window->win);
}

void window_set_mouse_mode(int mode) {
    SDL_SetRelativeMouseMode(mode);
}

void window_destroy(window_t *window) {
    if (!(window->flags & WINDOW_INITED)) {
        printf("WARN: Window not init'd\n");
        return;    
    }
    SDL_GL_DeleteContext(window->context);
    SDL_DestroyWindow(window->win);
}
