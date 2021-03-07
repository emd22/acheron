#ifndef AR_CM_ORTHO_H
#define AR_CM_ORTHO_H

#include <acheron/engine/scene/sc_camera.h>

typedef struct {
    float left, right, bottom, top;
    float near, far;
} ar_camera_ortho_info_t;

ar_camera_t ar_camera_ortho_new(void);


#endif