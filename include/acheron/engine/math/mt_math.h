#ifndef AR_M_MATH_H
#define AR_M_MATH_H

#include <acheron/engine/math/mt_vector.h>
#include <acheron/engine/math/mt_quat.h>
#include <acheron/engine/math/matrix3.h>
#include <acheron/engine/math/mt_mat4.h>

// TODO: remove legacy macros
#define math_deg_to_rad(degrees) ar_math_deg_to_rad(degrees)
#define math_rag_to_deg(rad) ar_math_rad_to_deg(rad)

#define AR_PI 3.1415926535

void ar_math_ortho(ar_mat4_t *mat, float l, float r, float b, float t, float n, float f);
ar_mat4_t ar_math_lookat(ar_vector3f_t from, ar_vector3f_t to, ar_vector3f_t upvec);
void ar_math_perspective(ar_mat4_t *mat, float fovy, float aspect, float znear, float zfar);

double ar_math_deg_to_rad(double degrees);
double ar_math_rad_to_deg(double rad);


#endif
