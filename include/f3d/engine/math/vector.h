#ifndef AR_MT_VECTOR_H
#define AR_MT_VECTOR_H

typedef enum {
    AR_VEC2,
    AR_VEC3,
    AR_VEC4,
    AR_VEC2F,
    AR_VEC3F,
    AR_VEC4F,
} ar_vector_type_t;

typedef struct {
    float x, y;
} ar_vector2f_t;

typedef struct {
    float x, y, z;
} ar_vector3f_t;

typedef struct {
    float x, y, z, w;
} ar_vector4f_t;

#endif
