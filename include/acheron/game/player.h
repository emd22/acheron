#ifndef PLAYER_H
#define PLAYER_H

#include <acheron/engine/camera/cm_perspective.h>

typedef struct {
    ar_camera_perspective_t *camera;
} player_t;

int player_move(player_t *player);

#endif
