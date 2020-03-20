#ifndef GAME_H
#define GAME_H

#include <f3d/game/controls.h>

#define GAME_IS_RUNNING     0x01

typedef struct {
    unsigned flags;
    controls_info_t controls;
} game_info_t;

extern game_info_t game_info;

void game_init();

#endif
