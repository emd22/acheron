#include <f3d/engine/type/matrix3.h>
#include <f3d/engine/type/vec.h>
#include <f3d/engine/core/log.h>

#include <string.h>

#define MAT3_INDEX(y, x) (y*3+x)

inline void mat3_set(mat3_t *mat, float *data) {
    mat->val[0] = data[0];
    mat->val[1] = data[1];
    mat->val[2] = data[2];
    mat->val[3] = data[3];
    mat->val[4] = data[4];
    mat->val[5] = data[5];
    mat->val[6] = data[6];
    mat->val[7] = data[7];
    mat->val[8] = data[8];
}

void mat3_identity(mat3_t *mat) {
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            mat->val[MAT3_INDEX(i, j)] = (i == j) ? 1.0f : 0.0f;
}

void mat3_sub(mat3_t *mat0, mat3_t mat1) {
    int i;
    for (i = 0; i < 3; i++)
        vec3f_sub((vector3f_t *)(mat0->val+i*3), *(vector3f_t *)(mat0->val+i*3), *(vector3f_t *)(mat1.val+i*3));
}

void mat3_add(mat3_t *mat0, mat3_t mat1) {
    int i;
    for (i = 0; i < 3; i++)
        vec3f_add((vector3f_t *)(mat0->val+i*3), *(vector3f_t *)(mat0->val+i*3), *(vector3f_t *)(mat1.val+i*3));
}

// TODO: pass in result matrix as pointer instead
mat3_t mat3_mul(mat3_t mat0, mat3_t mat1) {
    mat3_t res;
    int i, j, k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            res.val[MAT3_INDEX(i, j)] = 0;
            for (k = 0; k < 3; k++) {
                res.val[MAT3_INDEX(i, j)] += mat0.val[MAT3_INDEX(k, j)]*mat1.val[MAT3_INDEX(i, k)];
            }
        }
    }
    return res;
}

void mat3_set_inertia_tensor_coeffs(mat3_t *mat, float ix, float iy, float iz, float ixy, float ixz, float iyz) {
    mat->val[0] = ix;
    mat->val[1] = mat->val[3] = -ixy;
    mat->val[2] = mat->val[6] = -ixz;
    mat->val[4] = iy;
    mat->val[5] = mat->val[7] = -iyz;
    mat->val[8] = iz;
}

mat3_t mat3_inverse(mat3_t *m) {
    mat3_t res;

    const float t4 = m->val[0]*m->val[4];
    const float t6 = m->val[0]*m->val[5];
    const float t8 = m->val[1]*m->val[3];
    const float t10 = m->val[2]*m->val[3];
    const float t12 = m->val[1]*m->val[6];
    const float t14 = m->val[2]*m->val[6];
    
    // determinant
    float t16 = (t4*m->val[6] - t6*m->val[7] - t8*m->val[8] +
                 t10*m->val[7] + t12*m->val[5] - t14*m->val[4]);
    if (t16 == 0.0f)
        log_msg(LOG_ERROR, "determinant == 0\n", 0);
    
    float t17 = 1/t16;
    
    res.val[0] = (m->val[4]*m->val[8]-m->val[5]*m->val[7])*t17;
    res.val[1] = -(m->val[1]*m->val[8]-m->val[2]*m->val[7])*t17;
    res.val[2] = (m->val[1]*m->val[5]-m->val[2]*m->val[4])*t17;
    res.val[3] = -(m->val[3]*m->val[8]-m->val[5]*m->val[6])*t17;
    res.val[4] = (m->val[0]*m->val[8]-t14)*t17;
    res.val[5] = -(t6-t10)*t17;
    res.val[6] = (m->val[3]*m->val[7]-m->val[4]*m->val[6])*t17;
    res.val[7] = -(m->val[0]*m->val[7]-t12)*t17;
    res.val[8] = (t4-t8)*t17;
    return res;
}

