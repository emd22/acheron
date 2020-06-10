#ifndef F3D_WINDOW_H
#define F3D_WINDOW_H

#include <SDL2/SDL.h>

typedef enum {
    AR_WINDOW_OPTION_FULLSCREEN,
    AR_WINDOW_OPTION_MOUSE_VISIBLE,
} ar_window_option_t;

#define WINDOW_OPTION_MOUSE_VISIBLE 0x01

typedef struct {
    SDL_Window *win;
    SDL_GLContext context;
    
    int width, height;
    int flags;
} ar_window_t;

ar_window_t *ar_window_new(const char *title, int width, int height, int flags);
void ar_window_option_set(ar_window_t *window, int option, int value);
void ar_window_buffers_swap(ar_window_t *window);
void ar_window_destroy(ar_window_t *window);
void ar_window_buffer_destroy();

#endif
