#ifndef F3D_CAMERA_H
#define F3D_CAMERA_H

#include <f3d/engine/types.h>

#define CAMERA_FORWARD   0
#define CAMERA_BACKWARD  1
#define CAMERA_LEFT      2
#define CAMERA_RIGHT     3

typedef struct {
    vector3f_t position;
    vector3f_t rotation;
    
    vector3f_t right, direction, up;
    
    mat4_t mat_projection;
    
    float move_speed;
    
    float fov;
    
    mat4_t vp_mat;
} camera_t;

camera_t camera_new(void);
void camera_check(camera_t *_camera);
void camera_select(camera_t *camera);
void camera_clamp_rotation(camera_t *camera);
void camera_move(camera_t *camera, int direction);
void camera_update(camera_t *camera, unsigned shaderid);

#endif
