#include <gl/type/mat.h>
#include <gl/type/vec.h>
#include <gl/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void mat4_set(mat4_t *mat, float *data) {
    memcpy(mat->val, data, sizeof(float)*16);
}

mat4_t mat4_mul(mat4_t *mat0, mat4_t *mat1) {
    mat4_t res;
    int i;
    for (i = 0; i < 16; i++)
        res.val[i] = mat0->val[i]*mat1->val[i];
    return res;
}
