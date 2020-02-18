#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x, y, z;
    float pitch, yaw;
} camera_t;

camera_t camera_new(void);
void camera_check(camera_t *_camera);
void camera_select(camera_t *camera);
void camera_move(camera_t *_camera, int direction, float speed);
void camera_update(camera_t *_camera);

#endif
