#include <f3d/engine/core/window.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

static ar_buffer_t window_buffer;

ar_window_t *ar_window_new(const char *title, int width, int height, int flags) {
    if (window_buffer.initialized == false) {
        ar_buffer_init(&window_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_window_t), 1);
    }
    
    ar_window_t window;
    ar_log(AR_LOG_INFO, "Creating window '%s' with size %dx%d\n", title, width, height);
    window.win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    if (window.win == NULL) {
        ar_log(AR_LOG_ERROR, "Could not create SDL Window. Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    window.width = width;
    window.height = height;
    window.context = SDL_GL_CreateContext(window.win);
    if (window.context == NULL) {
        ar_log(AR_LOG_ERROR, "window.context == NULL!\n", 0);
    }
    
    // TODO: remove flags argument
    (void)flags;
    
    return ar_buffer_push(&window_buffer, &window);
}

void ar_window_buffers_swap(ar_window_t *window) {
    SDL_GL_SwapWindow(window->win);
}

void ar_window_option_set(ar_window_t *window, int option, int value) {
    if (option == AR_WINDOW_OPTION_FULLSCREEN) {
        const int mode = (value) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
        int width = window->width;
        int height = window->height;
        if (value) {
            SDL_DisplayMode mode;
            SDL_GetCurrentDisplayMode(0, &mode);
            width = mode.w;
            height = mode.h;
            SDL_SetWindowSize(window->win, width, height);
        }
        SDL_SetWindowFullscreen(window->win, mode);
    }
    else if (option == AR_WINDOW_OPTION_MOUSE_VISIBLE) {
        SDL_SetRelativeMouseMode(!value);
    }
}

void ar_window_destroy(ar_window_t *window) {
    SDL_GL_DeleteContext(window->context);
    SDL_DestroyWindow(window->win);
}
