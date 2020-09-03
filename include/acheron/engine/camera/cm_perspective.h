#ifndef AR_CM_PERSPECTIVE_H
#define AR_CM_PERSPECTIVE_H

#include <acheron/engine/scene/sc_camera.h>

typedef struct {
    ar_camera_t camera;
    float fov;

    float near, far;
} ar_camera_perspective_t;

ar_camera_perspective_t ar_camera_perspective_new(void);

#endif