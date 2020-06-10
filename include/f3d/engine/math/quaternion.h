#ifndef F3D_TYPE_QUATERNION_H
#define F3D_TYPE_QUATERNION_H

#include <f3d/engine/type/vec.h>
#include <f3d/engine/type/matrix4.h>

typedef struct {
    float w, x, y, z;
} quaternion_t;

void quaternion_from_mat4(quaternion_t *quat, mat4_t *mat);
void quaternion_to_mat4(quaternion_t *quat, mat4_t *mat);
void quaternion_convert_from_euler(quaternion_t *quat, vector3f_t euler);
quaternion_t quaternion_mul(quaternion_t *q0, quaternion_t *q1);

#endif
