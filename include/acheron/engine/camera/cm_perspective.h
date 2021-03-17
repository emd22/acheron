#ifndef AR_CM_PERSPECTIVE_H
#define AR_CM_PERSPECTIVE_H

#include <acheron/engine/scene/sc_camera.h>

typedef struct {
    float fov;
    float near, far;
} ar_camera_perspective_info_t;

ar_camera_t ar_camera_perspective_new(void);
int ar_camera_fov(ar_camera_t *camera, int value);

#endif