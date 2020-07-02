#ifndef F3D_TYPE_MATRIX4_H
#define F3D_TYPE_MATRIX4_H

#include <acheron/engine/math/mt_vector.h>

#define MAT4_IDENTITY \
    (float []){1, 0, 0, 0,/**/ 0, 1, 0, 0, /**/ 0, 0, 1, 0, /**/ 0, 0, 0, 1}

typedef struct {
    float val[16];
} mat4_t;

void mat4_set(mat4_t *mat, float *data);
void mat4_print(mat4_t *mat);
void mat4_scale(mat4_t *mat, mat4_t a, float k);
mat4_t mat4_rotate_x(mat4_t mat, float angle);
mat4_t mat4_rotate_y(mat4_t mat, float angle);
mat4_t mat4_rotate_z(mat4_t mat, float angle);
void mat4_translate_in_place(mat4_t *mat, ar_vector3f_t t);
void mat4_translate(mat4_t *mat, ar_vector3f_t v);
void mat4_add(mat4_t *mat0, mat4_t mat1);
mat4_t mat4_mul(mat4_t mat0, mat4_t mat1);
mat4_t mat4_mul_vec4(mat4_t mat, ar_vector4f_t vec);

#endif
