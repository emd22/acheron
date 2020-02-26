#include <gl/type/mat.h>
#include <gl/type/vec.h>
#include <gl/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAT4_INDEX(y, x) (y*4+x)

void mat4_set(mat4_t *mat, float *data) {
    memcpy(mat->val, data, sizeof(float)*16);
}

void mat4_rotate(mat4_t *mat, vector3f_t rotation) {
    
    const float crx = (rotation.x) ? cos(rotation.x) : 0;
    const float cry = (rotation.y) ? cos(rotation.y) : 0;
    const float crz = (rotation.z) ? cos(rotation.z) : 0;
    
    const float srx = sin(rotation.x);
    const float sry = sin(rotation.y);
    const float srz = sin(rotation.z);
    mat->val[0]  =  cry+crz;
    mat->val[1]  = -srz;
    mat->val[2]  =  sry;
    mat->val[4]  =  srz;
    mat->val[5]  =  crx+crz;
    mat->val[6]  = -srx;
    mat->val[8]  = -sry;
    mat->val[9]  =  srx;
    mat->val[10] =  crx+cry;

}

// https://github.com/datenwolf/linmath.h/blob/master/linmath.h
void mat4_row(vector4f_t *vec, mat4_t *mat, int i) {
    int k;
    for (k = 0; k < 4; k++)
        vec4f_set(vec, k, mat->val[MAT4_INDEX(k, i)]);
}

void mat4_translate_in_place(mat4_t *mat, vector3f_t t) {
    vector4f_t new = {t.x, t.y, t.z, 0};
    vector4f_t r;
    int i;
    for (i = 0; i < 4; i++) {
        mat4_row(&r, mat, i);
        mat->val[MAT4_INDEX(3, i)] += vec4f_mul_inner(r, new);
    }
}

mat4_t mat4_mul(mat4_t mat0, mat4_t mat1) {
    mat4_t res;
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            res.val[MAT4_INDEX(i, j)] = 0;
            for (k = 0; k < 4; k++) {
                res.val[MAT4_INDEX(i, j)] += mat0.val[MAT4_INDEX(k, j)]*mat1.val[MAT4_INDEX(i, k)];
            }
        }
    }
    return res;
}

mat4_t mat4_mul_vec4(mat4_t mat, vector4f_t vec) {
    mat4_t res;
    int i;
    for (i = 0; i < 16; i++)
        res.val[i] = mat.val[i]*vec4f_get(&vec, i % 4);
        
    return res;
}
