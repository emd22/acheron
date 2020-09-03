#ifndef AR_MT_MAT4_H
#define AR_MT_MAT4_H

#include <acheron/engine/math/mt_vector.h>

typedef struct {
    float val[16];
} ar_mat4_t;

void ar_mat4_set(ar_mat4_t *mat, float *data);
void ar_mat4_print(ar_mat4_t *mat);
void ar_mat4_scale(ar_mat4_t *mat, ar_mat4_t a, float k);
ar_mat4_t ar_mat4_rotate_x(ar_mat4_t mat, float angle);
ar_mat4_t ar_mat4_rotate_y(ar_mat4_t mat, float angle);
ar_mat4_t ar_mat4_rotate_z(ar_mat4_t mat, float angle);
void ar_mat4_translate_in_place(ar_mat4_t *mat, ar_vector3f_t t);
void ar_mat4_translate(ar_mat4_t *mat, ar_vector3f_t v);
void ar_mat4_add(ar_mat4_t *mat0, ar_mat4_t mat1);
ar_mat4_t ar_mat4_mul(ar_mat4_t mat0, ar_mat4_t mat1);
ar_mat4_t ar_mat4_mul_vec4(ar_mat4_t mat, ar_vector4f_t vec);

#endif