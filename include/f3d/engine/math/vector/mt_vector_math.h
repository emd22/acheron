#ifndef AR_MT_VECTOR_MATH_H
#define AR_MT_VECTOR_MATH_H

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

float ar_vector_dot(ar_vector_type_t type, void *vec0, void *vec1);
void ar_vector_scale(ar_vector_type_t type, void *vec, float s, void *result);
float ar_vector_length(ar_vector_type_t type, void *vec);
void ar_vector_normalize(ar_vector_type_t type, void *vec, void *result);
int ar_vector_compare(ar_vector_type_t type, void *vec0, void *vec1);

#endif
