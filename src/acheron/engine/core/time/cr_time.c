//typedef int temp_t;
#include <acheron/engine/core/cr_time.h>
#include <acheron/engine/core/cr_log.h>

#include <SDL2/SDL.h>

static float now, last;
static float delta_time = 0;

void ar_time_init(void) {
    now = SDL_GetTicks();
    last = 0;
}

void ar_time_tick(void) {
    last = now;
    now = SDL_GetTicks();
    // divide by 1000 to get seconds
    delta_time = (now-last)/1000.0f;
}

float ar_time_get_delta(void) {
    return delta_time;
}

void ar_time_end(void) {
}
