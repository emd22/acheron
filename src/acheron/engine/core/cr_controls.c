#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_controls.h>
#include <acheron/engine/core/cr_time.h>
#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_log.h>

#include <stdbool.h>

#include <SDL2/SDL.h>

#define AR_CONTROL_COOLDOWN_SPEED (0.6f)
#define AR_CONTROL_COOLDOWN_LENGTH (5.0f)

#define KEYDOWN 1
#define KEYUP   0

ar_controls_t controls;

void ar_controls_init(void) {
    memset(&controls, 0, sizeof(ar_controls_t));
}

ar_control_t *ar_control_get(int controln) {
    if (controln > AR_CONTROLS_MAX || controln < 0)
        return NULL;
    return &controls.controls[controln];
}

static void check_event(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        ar_instance_selected->running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        ar_control_t *control = ar_control_get(event->key.keysym.sym);
        if (control == NULL)
            return;
        control->pressed = true;
        control->modifiers = event->key.keysym.mod;
        //control->modifiers = SDL_GetModState();
    }
    else if (event->type == SDL_KEYUP) {
        ar_control_t *control = ar_control_get(event->key.keysym.sym);
        if (control == NULL)
            return;
        control->pressed = false;
        control->modifiers = event->key.keysym.mod;
        //control->modifiers = SDL_GetModState();
    }
    else if (event->type == SDL_MOUSEMOTION) {    
        ar_control_mouse_event_t mevent;
        
        mevent.x = event->motion.x;
        mevent.y = event->motion.y;
        mevent.relx = event->motion.xrel;
        mevent.rely = event->motion.yrel;
        ar_handle_call(AR_HANDLE_MOUSE_MOVE, &mevent);
    }
}

void ar_controls_poll_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        check_event(&event);
}

int ar_controls_get_modifiers(void) {
    return SDL_GetModState();
}

void ar_control_set_mode(int controln, ar_control_mode_t mode) {
    ar_control_get(controln)->mode = mode;
}

bool ar_control_check(int controln) {
    ar_control_t *control = ar_control_get(controln);
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
