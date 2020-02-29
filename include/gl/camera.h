#ifndef C3D_CAMERA_H
#define C3D_CAMERA_H

#include <gl/type/vec.h>

#define CAMERA_DIRECTION_FORWARD   1
#define CAMERA_DIRECTION_BACKWARD -1
#define CAMERA_DIRECTION_LEFT      2
#define CAMERA_DIRECTION_RIGHT     3

typedef struct {
    vector3f_t position;
    vector3f_t direction;
    vector3f_t rotation;
} camera_t;

camera_t camera_new(void);
void camera_check(camera_t *_camera);
void camera_select(camera_t *camera);
void camera_clamp_rotation(camera_t *camera);
void camera_move(camera_t *_camera, int direction, float speed);
void camera_update(camera_t *camera, unsigned shaderid);

#endif
