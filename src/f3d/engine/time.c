#include <f3d/engine/time.h>

#include <SDL2/SDL.h>

unsigned time_last;
unsigned time_fps;
unsigned time_frames;

void time_init(void) {
    time_last = SDL_GetTicks();
    time_frames = 0;
    time_fps = 0;
}

void time_tick(void) {
    time_frames++;
    if (time_last < SDL_GetTicks()-1.0f*1000) {
        time_last = SDL_GetTicks();
        time_fps = time_frames;
        time_frames = 0;
    }
}

unsigned time_get_fps(void) {
    return time_fps;
}
