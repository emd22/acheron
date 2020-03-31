#include <f3d/game/controls.h>
#include <f3d/game/game.h>

#include <SDL2/SDL.h>

int keys_pressed[CONTROLS_SIZE];

void controls_init(void) {
    int i;
    for (i = 0; i < CONTROLS_SIZE; i++)
        keys_pressed[i] = 0;
}

void controls_handle(int key, int state) {
    if (key == SDLK_w)
        keys_pressed[CONTROL_FORWARD] = state;
    else if (key == SDLK_s)
        keys_pressed[CONTROL_BACKWARD] = state;
    else if (key == SDLK_a)
        keys_pressed[CONTROL_LEFT] = state;
    else if (key == SDLK_d)
        keys_pressed[CONTROL_RIGHT] = state;
        
    else if (key >= SDLK_0 && key <= SDLK_9)
        keys_pressed[CONTROL_0+(key-SDLK_0)] = state;
        
    else if (key == SDLK_ESCAPE && state) {
        game_info.controls.flags ^= CONTROLS_MOUSE_CAPTURED;
        SDL_SetRelativeMouseMode(game_info.controls.flags & CONTROLS_MOUSE_CAPTURED);
    }
    else if (key == SDLK_q) {
        game_info.flags &= ~GAME_IS_RUNNING;
    }
}

void controls_handle_keydown(int key) {
    controls_handle(key, 1);
}

void controls_handle_keyup(int key) {
    controls_handle(key, 0);
}
