#include <f3d/engine/core/controls.h>
#include <f3d/engine/core/time.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/acheron.h>

#include <stdbool.h>

#include <SDL2/SDL.h>

#define AR_CONTROL_COOLDOWN_SPEED (0.6f)
#define AR_CONTROL_COOLDOWN_LENGTH (5.0f)

#define KEYDOWN 1
#define KEYUP   0

ar_controls_t controls;

void mouse_move_dummy(SDL_Event *event) {
    (void)event;
}

void ar_controls_init(void) {
    memset(&controls, 0, sizeof(ar_controls_t));
    controls.mouse_move_func = &mouse_move_dummy;
}

void ar_controls_set_mouse_func(void (*mouse_move_func)(SDL_Event *)) {
    controls.mouse_move_func = mouse_move_func;
}

ar_control_t *ar_control_get(int controln) {
    return &controls.controls[controln];
}

static void check_event(SDL_Event *event) {
    ar_instance_t *instance = ar_instance_get_selected();
    if (instance == NULL)
        return;
        
    if (event->type == SDL_QUIT) {
        ar_instance_selected->running = false;
    }
    else if (event->type == SDL_KEYDOWN) {
        ar_control_t *control = ar_control_get(event->key.keysym.sym);
        control->pressed = true;
        control->modifiers = event->key.keysym.mod; 
    }
    else if (event->type == SDL_KEYUP) {
        ar_control_t *control = ar_control_get(event->key.keysym.sym);
        control->pressed = false;
        control->modifiers = event->key.keysym.mod;
    }
    else if (event->type == SDL_MOUSEMOTION) {
        controls.mouse_move_func(event); // (event->motion.xrel, event->motion.yrel)
    }
}

void ar_controls_poll_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        check_event(&event);
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

void controls_update(void) {
    //int i;
    //ar_control_t *key;
    //for (i = 0; i < AR_CONTROLS_MAX; i++) {
        //key = &controls.controls[i];
        //if (key->cooldown > 0.0f) {
            //key->cooldown -= key->cool_speed*delta_time;
        //}
    //}
}
