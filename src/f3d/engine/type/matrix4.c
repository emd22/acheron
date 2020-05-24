#include <f3d/engine/type/matrix4.h>
#include <f3d/engine/type/vec.h>
#include <f3d/engine/core/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAT4_INDEX(y, x) (y*4+x)

// TODO: Some optimizations and loop unrolling
// code should be changed to modify pointer instead of returning new matrix.

inline void mat4_set(mat4_t *mat, float *data) {
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
void mat4_identity(mat4_t *mat) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat->val[MAT4_INDEX(i, j)] = (i == j) ? 1.0f : 0.0f;
}

void mat4_row(vector4f_t *vec, mat4_t *mat, int i) {
    int k;
    for (k = 0; k < 4; k++)
        vecf_set_at(TYPE_VEC4F, vec, k, mat->val[MAT4_INDEX(k, i)]);
}

void mat4_print(mat4_t *mat) {
    int i;
    printf("=== MAT DEBUG ===\n");
    for (i = 0; i < 16; i++) {
        printf("%.02f ", mat->val[i]);
        if (!((i+1) % 4))
            printf("\n");
    }
    printf("=== MAT DEBUG END ===\n");
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
        mat->val[MAT4_INDEX(3, i)] += vec4f_dot(r, new);
    }
}

void mat4_sub(mat4_t *mat0, mat4_t mat1) {
    int i;
    for (i = 0; i < 4; i++)
        vec4f_sub((vector4f_t *)(mat0->val+i*4), *(vector4f_t *)(mat0->val+i*4), *(vector4f_t *)(mat1.val+i*4));
}

void mat4_add(mat4_t *mat0, mat4_t mat1) {
    int i;
    for (i = 0; i < 4; i++)
        vec4f_add((vector4f_t *)(mat0->val+i*4), *(vector4f_t *)(mat0->val+i*4), *(vector4f_t *)(mat1.val+i*4));
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
        res.val[i] = mat.val[i]*vecf_get_at(TYPE_VEC4F, &vec, i % 4);
        
    return res;
}

void mat4_from_vec3_mul_outer(mat4_t *mat, vector3f_t a, vector3f_t b) {
	int i, j;
	for (i = 0; i < 4; i++) 
	    for (j = 0; j < 4; j++)
		    mat->val[MAT4_INDEX(i, j)] = (i < 3 && j < 3) 
		                    ? vecf_get_at(TYPE_VEC3F, &a, i)*vecf_get_at(TYPE_VEC3F, &b, j) 
		                    : 0.0f;
}

void mat4_scale(mat4_t *mat, mat4_t a, float k) {
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
        mat4_t c;
        mat4_identity(&c);
        mat4_sub(&c, t);
        mat4_scale(&c, c, cn);
        mat4_add(&t, c);
        mat4_add(&t, s);
        t.val[15] = 1;
        res = mat4_mul(*mat, t);
        return res;
    }
    else {
        return *mat;
    }
    return res;
}

mat4_t mat4_rotate_x(mat4_t mat, float angle) {
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
    res = mat4_mul(mat, rot);
    return res;
}

mat4_t mat4_rotate_y(mat4_t mat, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    mat4_t rot;
    mat4_set(
        &rot,
        (float []){
              cn, 0.0f,   sn, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
             -sn, 0.0f,   cn, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    rot = mat4_mul(mat, rot);
    return rot;
}

mat4_t mat4_rotate_z(mat4_t mat, float angle) {
    const float sn = sinf(angle);
    const float cn = cosf(angle);
    
    mat4_t rot;
    mat4_set(
        &rot,
        (float []){
              cn,   sn, 0.0f, 0.0f,
             -sn,   cn, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    rot = mat4_mul(mat, rot);
    return rot;
}
