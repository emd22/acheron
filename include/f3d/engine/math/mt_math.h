#ifndef AR_M_MATH_H
#define AR_M_MATH_H

#include <f3d/engine/math/mt_vector.h>
#include <f3d/engine/math/matrix3.h>
#include <f3d/engine/math/matrix4.h>

void math_ortho(mat4_t *mat, float l, float r, float b, float t, float n, float f);
mat4_t math_lookat(ar_vector3f_t from, ar_vector3f_t to, ar_vector3f_t upvec);
void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar);

double math_deg_to_rad(double degrees);
double math_rad_to_deg(double rad);

#endif
