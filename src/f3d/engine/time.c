#include <f3d/engine/time.h>

#include <SDL2/SDL.h>

static unsigned last;
static unsigned fps;
static unsigned frames;

void time_init(void) {
    last = SDL_GetTicks();
    frames = 0;
    fps = 0;
}

void time_tick(void) {
    frames++;
    if (last < SDL_GetTicks()-1.0f*1000) {
        last = SDL_GetTicks();
        fps = frames;
        frames = 0;
    }
}

unsigned time_get_fps(void) {
    return fps;
}
