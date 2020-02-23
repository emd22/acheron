#ifndef C3D_TYPE_MAT_H
#define C3D_TYPE_MAT_H

#include <gl/type/vec.h>

typedef struct {
    float val[16];
} mat4_t;

void mat4_set(mat4_t *mat, float *data);
mat4_t mat4_mul(mat4_t mat0, mat4_t mat1);
mat4_t mat4_mul_vec4(mat4_t mat, vector4f_t vec);

#endif
