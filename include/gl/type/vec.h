#ifndef C3D_TYPE_VEC_H
#define C3D_TYPE_VEC_H

// float
typedef struct {
    float w, x, y, z;
} __attribute__((packed)) vector4f_t;

typedef struct {
    float x, y, z;
} __attribute__((packed)) vector3f_t;

// int type
typedef struct {
    int w, x, y, z;
} vector4_t;

float vec4f_get(vector4f_t *vec, int index);
vector3f_t vec3f_sub_vec3f(vector3f_t vec0, vector3f_t vec1);

#endif
