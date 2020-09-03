#ifndef AR_SC_CAMERA_H
#define AR_SC_CAMERA_H

#include <acheron/engine/types.h>

typedef struct ar_camera_s {
    ar_vector3f_t position;
    ar_vector3f_t rotation;

    ar_vector3f_t direction;
    ar_vector3f_t up, right;

    ar_mat4_t view, projection;

    void *(*update)(struct ar_camera_s *, void *);
    void *(*reload)(struct ar_camera_s *, void *);
} ar_camera_t;

ar_camera_t ar_camera_new(void);
void ar_camera_clamp(ar_camera_t *camera, ar_vector3f_t axis, float min_r, float max_r);
void ar_camera_rotate(ar_camera_t *camera, ar_vector3f_t value);
void ar_camera_update(ar_camera_t *camera);

#endif