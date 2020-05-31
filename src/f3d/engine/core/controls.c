#include <f3d/engine/core/controls.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/acheron.h>

#include <stdbool.h>

#include <SDL2/SDL.h>

#define AR_CONTROL_COOLDOWN_SPEED (0.8f)
#define AR_CONTROL_COOLDOWN_LENGTH (5.0f)

#define KEYDOWN 1
#define KEYUP   0

ar_controls_t controls;

void controls_init(void) {
    memset(&controls, 0, sizeof(ar_controls_t));
}

static void check_event(SDL_Event *event) {
    sb_instance_t *instance = ar_instance_get_selected();
    if (instance == NULL)
        return;
        
    if (event->type == SDL_QUIT) {
        ar_instance_selected->running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        ar_control_t *control = &controls.keys[event->key.keysym.sym];
        control->pressed = true;
        control->modifiers = event->key.keysym.mod; 
    }
    else if (event->type == SDL_KEYUP) {
        ar_control_t *control = &controls.keys[event->key.keysym.sym];
        control->pressed = false;
        control->modifiers = event->key.keysym.mod;
    }
    else if (event->type == SDL_MOUSEMOTION) {
        check_mouse(event->motion.xrel, event->motion.yrel);
    }
}

void ar_controls_poll_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        check_event(&event);
}

bool ar_control_check(int controln) {
    ar_control_t *control = ar_instance_selected->controls[controln];
    if (control->cooldown > 0.0f) {
        // key was just pressed, so we return pressed value
        if (control->pressed) {
            control->pressed = false;
            return true;
        }
        control->cooldown -= (AR_CONTROL_COOLDOWN_SPEED*ar_time_get_delta());
    }
    else {
        if (control->mode == AR_CONTROL_MODE_TOGGLE && control->pressed) {
            control->cooldown = AR_CONTROL_COOLDOWN_LENGTH;
        }
    }
    return control->pressed;
}

void controls_update(void) {
    int i;
    control_key_t *key;
    for (i = 0; i < CONTROLS_SIZE; i++) {
        key = &keys[i];
        if (key->cooldown > 0.0f) {
            key->cooldown -= key->cool_speed*delta_time;
        }
    }
}
