#include <gl/type/mat.h>
#include <gl/type/vec.h>
#include <gl/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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



mat4_t mat4_mul(mat4_t mat0, mat4_t mat1) {
    mat4_t res;
    int i;
    for (i = 0; i < 16; i++)
        res.val[i] = mat0.val[i]*mat1.val[i];
    return res;
}

mat4_t mat4_mul_vec4(mat4_t mat, vector4f_t vec) {
    mat4_t res;
    int i;
    for (i = 0; i < 16; i++)
        res.val[i] = mat.val[i]*vec4f_get(&vec, i % 4);
        
    return res;
}
