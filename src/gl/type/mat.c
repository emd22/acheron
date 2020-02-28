#include <gl/type/mat.h>
#include <gl/type/vec.h>
#include <gl/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAT4_INDEX(y, x) (y*4+x)

// TODO: Some optimizations and loop unrolling
// code should be changed to modify pointer instead of returning new matrix.

void mat4_set(mat4_t *mat, float *data) {
    memcpy(mat->val, data, sizeof(float)*16);
}

// https://github.com/datenwolf/linmath.h/blob/master/linmath.h
void mat4_identity(mat4_t *mat) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat->val[MAT4_INDEX(i, j)] = (i == j) ? 1.0f : 0.0f;
}

void mat4_row(vector4f_t *vec, mat4_t *mat, int i) {
    int k;
    for (k = 0; k < 4; k++)
        vecf_set_at(vec, TYPE_VEC4F, k, mat->val[MAT4_INDEX(k, i)]);
}

void mat4_translate(mat4_t *mat, vector3f_t v) {
    mat4_identity(mat);
    mat->val[MAT4_INDEX(3, 0)] = v.x;
    mat->val[MAT4_INDEX(3, 1)] = v.y;
    mat->val[MAT4_INDEX(3, 2)] = v.z;
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

// TODO: pass in result matrix as pointer instead
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
        res.val[i] = mat.val[i]*vecf_get_at(&vec, TYPE_VEC4F, i % 4);
        
    return res;
}

void mat4_from_vec3_mul_outer(mat4_t *mat, vector3f_t a, vector3f_t b) {
	int i, j;
	for (i = 0; i < 4; i++) 
	    for (j = 0; j < 4; j++)
		    mat->val[MAT4_INDEX(i, j)] = (i < 3 && j < 3) 
		                    ? vecf_get_at(&a, TYPE_VEC3F, i)*vecf_get_at(&b, TYPE_VEC3F, j) 
		                    : 0.0f;
}

void mat4_scale(mat4_t *mat, mat4_t a, float k) {
    int i;
	for (i = 0; i < 4; i++)
		vec4f_scale((vector4f_t *)(mat->val+(i*4)), *((vector4f_t *)(a.val+(i*4))), k);
}

mat4_t mat4_rotate(mat4_t *mat, vector3f_t r, float angle) {
    float sn = sinf(angle);
    float cn = cosf(angle);
    (void)cn;
    
    mat4_t res, t;
    
    if (vec3f_len(r) > 1e-4) {
        vec3f_norm(&r, r);
        mat4_t s;
        mat4_set(
            &s, 
            (float []){
                 0.0f,  r.z,  -r.y,  0.0f,
                -r.z,   0.0f,  r.x,  0.0f,
                 r.y,  -r.x,   0.0f, 0.0f,
                 0.0f,  0.0f,  0.0f, 0.0f
                
            }
        );
        mat4_from_vec3_mul_outer(&t, r, r);
        mat4_scale(&s, s, sn);
    }
    else {
        return *mat;
    }
    return res;
}

mat4_t mat4_rotate_x(mat4_t mat1, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    mat4_t rot, res;
    mat4_set(
        &rot, 
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,   cn,   sn, 0.0f,
            0.0f,  -sn,   cn, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    res = mat4_mul(mat1, rot);
    return res;
}
