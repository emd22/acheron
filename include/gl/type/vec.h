#ifndef C3D_TYPE_VEC_H
#define C3D_TYPE_VEC_H

// float
typedef struct {
    float x, y, z, w;
} vector4f_t;

typedef struct {
    float x, y, z;
} vector3f_t;

// int type
typedef struct {
    int x, y, z, w;
} vector4_t;

typedef struct {
    int x, y, z;
} vector3_t;

float vec4f_get(vector4f_t *vec, int index);
void vec4f_set(vector4f_t *vec, int index, float value);
float vec4f_mul_inner(vector4f_t a, vector4f_t b);
vector3f_t vec3f_sub_vec3f(vector3f_t vec0, vector3f_t vec1);

#endif
