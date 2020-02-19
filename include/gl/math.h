#ifndef C3D_MATH_H
#define C3D_MATH_H

typedef struct {
    float val[16];
} mat4_t;

void math_perspective_m4(mat4_t *mat, float fovy, float aspect, float znear, float zfar);
void math_frustum_m4(mat4_t *mat, float left, float right, float bottom, float top, float znear, float zfar);

#endif
