#include <acheron/engine/math/mt_mat4.h>
#include <acheron/engine/math/mt_vector.h>
#include <acheron/engine/core/cr_log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAT4_INDEX(y, x) (y*4+x)

// TODO: Some optimizations and loop unrolling
// code should be changed to modify pointer instead of returning new matrix.

inline void ar_mat4_set(ar_mat4_t *mat, float *data) {
    mat->val[0] = data[0];
    mat->val[1] = data[1];
    mat->val[2] = data[2];
    mat->val[3] = data[3];
    mat->val[4] = data[4];
    mat->val[5] = data[5];
    mat->val[6] = data[6];
    mat->val[7] = data[7];
    mat->val[8] = data[8];
    mat->val[9] = data[9];
    mat->val[10] = data[10];
    mat->val[11] = data[11];
    mat->val[12] = data[12];
    mat->val[13] = data[13];
    mat->val[14] = data[14];
    mat->val[15] = data[15];
}

// https://github.com/datenwolf/linmath.h/blob/master/linmath.h
void ar_mat4_identity(ar_mat4_t *mat) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat->val[MAT4_INDEX(i, j)] = (i == j) ? 1.0f : 0.0f;
}

void ar_mat4_row(ar_vector4f_t *vec, ar_mat4_t *mat, int i) {
    int k;
    for (k = 0; k < 4; k++)
        ar_vectorf_set(AR_VEC4F, vec, k, mat->val[MAT4_INDEX(k, i)]);
}

void ar_mat4_print(ar_mat4_t *mat) {
    int i;
    printf("=== MAT DEBUG ===\n");
    for (i = 0; i < 16; i++) {
        printf("%.02f ", mat->val[i]);
        if (!((i+1) % 4))
            printf("\n");
    }
    printf("=== MAT DEBUG END ===\n");
}

void ar_mat4_translate(ar_mat4_t *mat, ar_vector3f_t v) {
    ar_mat4_identity(mat);
    mat->val[MAT4_INDEX(3, 0)] = v.x;
    mat->val[MAT4_INDEX(3, 1)] = v.y;
    mat->val[MAT4_INDEX(3, 2)] = v.z;
}

void ar_mat4_translate_in_place(ar_mat4_t *mat, ar_vector3f_t t) {
    ar_vector4f_t new = {t.x, t.y, t.z, 0};
    ar_vector4f_t r;
    int i;
    for (i = 0; i < 4; i++) {
        ar_mat4_row(&r, mat, i);
        mat->val[MAT4_INDEX(3, i)] += ar_vector_dot(AR_VEC4F, &r, &new);
    }
}

void ar_mat4_sub(ar_mat4_t *mat0, ar_mat4_t mat1) {
    int i;
    for (i = 0; i < 4; i++)
        ar_vector_sub(AR_VEC4F, (mat0->val+i*4), (mat1.val+i*4), (mat0->val+i*4));
}

void ar_mat4_add(ar_mat4_t *mat0, ar_mat4_t mat1) {
    int i;
    for (i = 0; i < 4; i++)
        ar_vector_add(AR_VEC4F, (mat0->val+i*4), (mat1.val+i*4), (mat0->val+i*4));
}

// TODO: pass in result matrix as pointer instead
ar_mat4_t ar_mat4_mul(ar_mat4_t mat0, ar_mat4_t mat1) {
    ar_mat4_t res;
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

ar_mat4_t ar_mat4_mul_vec4(ar_mat4_t mat, ar_vector4f_t vec) {
    ar_mat4_t res;
    int i;
    for (i = 0; i < 16; i++)
        res.val[i] = mat.val[i]*ar_vectorf_get(AR_VEC4F, &vec, i % 4);
        
    return res;
}

void ar_mat4_from_vec3_mul_outer(ar_mat4_t *mat, ar_vector3f_t a, ar_vector3f_t b) {
    int i, j;
    for (i = 0; i < 4; i++) 
        for (j = 0; j < 4; j++)
            mat->val[MAT4_INDEX(i, j)] = (i < 3 && j < 3) 
                            ? ar_vectorf_get(AR_VEC3F, &a, i)*ar_vectorf_get(AR_VEC3F, &b, j) 
                            : 0.0f;
}

void ar_mat4_scale(ar_mat4_t *mat, ar_mat4_t a, float k) {
    int i;
    for (i = 0; i < 4; i++) {
        mat->val[i] = a.val[i]*k;
        mat->val[i+1] = a.val[i+1]*k;
        mat->val[i+2] = a.val[i+2]*k;
        mat->val[i+3] = a.val[i+3]*k;
    }
    mat->val[15] = 1;
        //vec4f_scale((vector4f_t *)(mat->val+(i*4)), *((vector4f_t *)(a.val+(i*4))), k);
}

ar_mat4_t ar_mat4_rotate(ar_mat4_t *mat, ar_vector3f_t r, float angle) {
    float sn = sinf(angle);
    float cn = cosf(angle);
    (void)cn;
    
    ar_mat4_t res, t;
    
    if (ar_vector_length(AR_VEC3F, &r) > 1e-4) {
        ar_vector_normalize(AR_VEC3F, &r, &r);
        ar_mat4_t s;
        ar_mat4_set(
            &s, 
            (float []){
                 0.0f,  r.z,  -r.y,  0.0f,
                -r.z,   0.0f,  r.x,  0.0f,
                 r.y,  -r.x,   0.0f, 0.0f,
                 0.0f,  0.0f,  0.0f, 0.0f
            }
        );
        ar_mat4_from_vec3_mul_outer(&t, r, r);
        
        ar_mat4_scale(&s, s, sn);
        ar_mat4_t c;
        ar_mat4_identity(&c);
        ar_mat4_sub(&c, t);
        ar_mat4_scale(&c, c, cn);
        ar_mat4_add(&t, c);
        ar_mat4_add(&t, s);
        t.val[15] = 1;
        res = ar_mat4_mul(*mat, t);
        return res;
    }
    else {
        return *mat;
    }
    return res;
}

ar_mat4_t ar_mat4_rotate_x(ar_mat4_t mat, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    ar_mat4_t rot, res;
    ar_mat4_set(
        &rot, 
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,   cn,   sn, 0.0f,
            0.0f,  -sn,   cn, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    res = ar_mat4_mul(mat, rot);
    return res;
}

ar_mat4_t ar_mat4_rotate_y(ar_mat4_t mat, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    ar_mat4_t rot;
    ar_mat4_set(
        &rot,
        (float []){
              cn, 0.0f,   sn, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
             -sn, 0.0f,   cn, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    rot = ar_mat4_mul(mat, rot);
    return rot;
}

ar_mat4_t ar_mat4_rotate_z(ar_mat4_t mat, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    ar_mat4_t rot;
    ar_mat4_set(
        &rot,
        (float []){
              cn,   sn, 0.0f, 0.0f,
             -sn,   cn, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    rot = ar_mat4_mul(mat, rot);
    return rot;
}
