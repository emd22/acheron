#ifndef C3D_TYPE_MAT_H
#define C3D_TYPE_MAT_H

#include <f3d/engine/type/vec.h>

#define MAT4_IDENTITY \
    (float []){1, 0, 0, 0,/**/ 0, 1, 0, 0, /**/ 0, 0, 1, 0, /**/ 0, 0, 0, 1}

typedef struct {
    float val[16];
} mat4_t;

void mat4_set(mat4_t *mat, float *data);
mat4_t mat4_rotate_x(mat4_t mat, float angle);
mat4_t mat4_rotate_y(mat4_t mat, float angle);
mat4_t mat4_rotate_z(mat4_t mat, float angle);
mat4_t mat4_rotate(mat4_t *mat, vector3f_t r, float angle);
void mat4_translate_in_place(mat4_t *mat, vector3f_t t);
void mat4_translate(mat4_t *mat, vector3f_t v);
void mat4_add(mat4_t *mat0, mat4_t mat1);
mat4_t mat4_mul(mat4_t mat0, mat4_t mat1);
mat4_t mat4_mul_vec4(mat4_t mat, vector4f_t vec);

#endif
