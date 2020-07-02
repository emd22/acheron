#ifndef F3D_TYPE_MATRIX3_H
#define F3D_TYPE_MATRIX3_H

typedef struct {
    float val[9];
} mat3_t;

void mat3_set(mat3_t *mat, float *data);
void mat3_set_inertia_tensor_coeffs(mat3_t *mat, float ix, float iy, float iz, float ixy, float ixz, float iyz);

#endif
