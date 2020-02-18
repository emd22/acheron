#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#define WINDOW_INITED 0x01

#define WINDOW_MOUSE_DISABLED 1
#define WINDOW_MOUSE_ENABLED  0

typedef struct {
    SDL_Window *win;
    SDL_GLContext context;
    int width, height;
    int flags;
} window_t;

window_t window_new(const char *title, int width, int height, int flags);
void window_set_default(window_t *window);
window_t *window_get_default(void);
void window_buffers_swap(window_t *window);
void window_set_mouse_mode(int mode);
void window_destroy(window_t *window);

#endif
