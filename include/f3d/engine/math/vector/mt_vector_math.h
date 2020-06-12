#ifndef AR_MT_VECTOR_MATH_H
#define AR_MT_VECTOR_MATH_H

#define VEC2F(val) (ar_vector2f_t){val, val}
#define VEC3F(val) (ar_vector3f_t){val, val, val}
#define VEC4F(val) (ar_vector4f_t){val, val, val, val}

// if vector is float type(vec2f, vec3f, vec4f) return size of float; 
// else return sizeof int
#define _AR_VEC_ELEMENT_SIZE(type) ((type >= AR_VEC2F) ? sizeof(float) : sizeof(int))
// get the amount of elements in vector of type n
#define _AR_VEC_LENGTH(type) (ar_vector_type_size(type)/_AR_VEC_ELEMENT_SIZE(type))

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

/*
 * Vector type functions
 */
int ar_vector_type_offset(ar_vector_type_t type);
int ar_vector_type_size(ar_vector_type_t type);

/*
 * Vector get/set functions
 */
void ar_vectorf_set(ar_vector_type_t type, void *vec, int index, float value);
void ar_vectori_set(ar_vector_type_t type, void *vec, int index, int value);
float ar_vectorf_get(ar_vector_type_t type, void *vec, int index);
int ar_vectori_get(ar_vector_type_t type, void *vec, int index);

/*
 * Vector math functions
 */
float ar_vector_dot(ar_vector_type_t type, void *vec0, void *vec1);
void  ar_vector_scale(ar_vector_type_t type, void *vec, float s, void *result);
float ar_vector_length(ar_vector_type_t type, void *vec);
void  ar_vector_normalize(ar_vector_type_t type, void *vec, void *result);
int   ar_vector_compare(ar_vector_type_t type, void *vec0, void *vec1);

#endif
