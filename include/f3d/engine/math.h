#ifndef F3D_MATH_H
#define F3D_MATH_H

#include <f3d/engine/types.h>

#define F3D_PI 3.1415926535

inline double math_deg_to_rad(double degrees) {
    return degrees/57.295779513;
}

inline double math_rad_to_deg(double rad) {
    return rad*57.295779513;
}

vector3f_t math_cross(vector3f_t vec0, vector3f_t vec1);
float math_dot(vector3f_t vec0, vector3f_t vec1);
mat4_t math_lookat(vector3f_t from, vector3f_t to, vector3f_t upvec);
void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar);
void math_frustum_mat4(mat4_t *mat, float left, float right, float bottom, float top, float znear, float zfar);

#endif
