#ifndef C3D_MATH_H
#define C3D_MATH_H

#include <gl/types.h>

void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar);
void math_frustum_mat4(mat4_t *mat, float left, float right, float bottom, float top, float znear, float zfar);

#endif
