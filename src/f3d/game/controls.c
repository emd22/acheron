#include <f3d/game/controls.h>
#include <f3d/engine/engine.h>
#include <f3d/game/game.h>

#include <stdbool.h>

#include <SDL2/SDL.h>

#define SINGLE_PRESS_COOLDOWN 1.0f

#define KEYDOWN 1
#define KEYUP   0

control_key_t keys[CONTROLS_SIZE];

void controls_init(void) {
    int i;
    for (i = 0; i < CONTROLS_SIZE; i++) {
        keys[i].pressed = 0;
        keys[i].cooldown = 0;
        keys[i].cool_speed = 5.0f;    
    }
}

bool check_cooldown(control_key_t *key) {
    if (key->cooldown <= 0.0f) {
        key->cooldown = SINGLE_PRESS_COOLDOWN;
        return false;        
    }
    return true;
}

void controls_handle(int key, int state) {
    if (key == SDLK_w)
        keys[CONTROL_FORWARD].pressed = state;
    else if (key == SDLK_s)
        keys[CONTROL_BACKWARD].pressed = state;
    else if (key == SDLK_a)
        keys[CONTROL_LEFT].pressed = state;
    else if (key == SDLK_d)
        keys[CONTROL_RIGHT].pressed = state;
        
    else if (key >= SDLK_0 && key <= SDLK_9) {
        control_key_t *nkey = &(keys[CONTROL_0+(key-SDLK_0)]);
        if (!check_cooldown(nkey)) {
            nkey->pressed = state;
        }
    }
    else if (key == SDLK_ESCAPE && state) {
        game_info.controls.flags ^= CONTROLS_MOUSE_CAPTURED;
        SDL_SetRelativeMouseMode(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED);
    }
    else if (key == SDLK_q) {
        game_info.flags &= ~GAME_IS_RUNNING;
    }
}

bool controls_check_toggle(int key) {
    control_key_t *nkey = &keys[key];
    // cooldown was set
    if (nkey->cooldown > 0.0) {
        // key was just pressed, so we return pressed value
        if (nkey->pressed) {
            // set pressed to false so it won't return again
            nkey->pressed = false;
            return true;     
        }
        
        nkey->cooldown -= nkey->cool_speed*delta_time;
    }
    
    return false;
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

void controls_handle_keydown(int key) {
    controls_handle(key, KEYDOWN);
}

void controls_handle_keyup(int key) {
    controls_handle(key, KEYUP);
}
