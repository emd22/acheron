#ifndef AR_MT_QUATERNION_H
#define AR_MT_QUATERNION_H

#include <acheron/engine/math/mt_vector.h>
#include <acheron/engine/math/matrix4.h>

typedef struct {
    float w, x, y, z;
} ar_quat_t;

void ar_quat_from_mat4(ar_quat_t *quat, mat4_t *mat);
void ar_quat_to_mat4(ar_quat_t *quat, mat4_t *mat);
void ar_quat_from_euler(ar_quat_t *quat, ar_vector3f_t euler);
ar_quat_t ar_quat_mul(ar_quat_t *q0, ar_quat_t *q1);

#endif
