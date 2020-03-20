#include <f3d/engine/time.h>

#include <SDL2/SDL.h>

static unsigned now, last;
double delta_time = 0;

void time_init(void) {
    now = SDL_GetPerformanceCounter();
    last = 0;
}

void time_tick(void) {
    last = now;
    now = SDL_GetPerformanceCounter();
    delta_time = (double)((now-last)*1000/(double)SDL_GetPerformanceFrequency());
}
