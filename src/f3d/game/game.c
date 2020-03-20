#include <f3d/game/game.h>
#include <f3d/game/controls.h>

game_info_t game_info;

void game_init(void) {
    game_info.flags = 0;
    game_info.controls.flags = 0;
    controls_init();
    game_info.controls.flags |= CONTROLS_MOUSE_CAPTURED;
}
