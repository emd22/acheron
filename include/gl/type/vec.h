#ifndef C3D_TYPE_VEC_H
#define C3D_TYPE_VEC_H

// float
typedef struct {
    float w, x, y, z;
} __attribute__((packed)) vec4f_t;

typedef struct {
    float x, y, z;
} __attribute__((packed)) vec3f_t;

// int type
typedef struct {
    int w, x, y, z;
} vec4_t;

void vec4f_get_data(vec4f_t *vec, float *data);

#endif
