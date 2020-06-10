#include <f3d/engine/math/vector.h>

#include <stddef.h>
#include <stdint.h>

// if float type(vec2f, 3f, 4f) return sizeof float; else return sizeof int
#define VEC_ELEMENT_SIZE(type) ((type >= AR_VEC2F) ? sizeof(float) : sizeof(int))
// get the amount of elements in vector of type n
#define VEC_LENGTH(type) (get_vector_type_size(type)/VEC_ELEMENT_SIZE(type)))

static int get_vector_type_offset(ar_vector_type_t type) {
    switch (type) {
        case AR_VEC2F:
            return offsetof(ar_vector2f_t, x);
        case AR_VEC3F:
            return offsetof(ar_vector3f_t, x);
        case AR_VEC4F:
            return offsetof(ar_vector4f_t, x);
    }
    return 0;
}

static int get_vector_type_size(ar_vector_type_t type) {
    switch (type) {
        case AR_VEC2F:
            return sizeof(ar_vector2f_t);
        case AR_VEC3F:
            return sizeof(ar_vector3f_t);
        case AR_VEC4F:
            return sizeof(ar_vector4f_t);
    }
    return 0;
}

/*
 * Float and integer get/set functions
 */


void ar_vectorf_set(ar_vector_type_t type, void *vec, int index, float value) {
    const int length = VEC_LENGTH(type);
    if (index < 0 || index > length)
        return;
    float *valp = (float *)(vec+(sizeof(float)*index));
    (*valp) = value;
}

void ar_vectori_set(ar_vector_type_t type, void *vec, int index, int value) {
    const int length = VEC_LENGTH(type);
    if (index < 0 || index > length)
        return;
    int *valp = (int *)(vec+(sizeof(int)*index));
    (*valp) = value;
}

float ar_vectorf_get(ar_vector_type_t type, void *vec, int index) {
    const int length = VEC_LENGTH(type);
    if (index < 0 || index > length)
        return 0.0f;
    float *value = (float *)(vec+(sizeof(float)*index));
    return (*value);
}

int ar_vectori_get(ar_vector_type_t type, void *vec, int index) {
    const int length = VEC_LENGTH(type);
    if (index < 0 || index > length)
        return 0;
    int *value = (int *)(vec+sizeof(int)*index);
    return (*value);
}

/*
 * Vector math functions
 */

void ar_vector_dot(ar_vector_type_t type, void *vec0, void *vec1) {
    const int length = VEC_LENGTH(type);
    
    float p = 0.0f;
    int i;
    for (i = 0; i < length; i++)
        p += ar_vectorf_get(type, vec0, i)*ar_vectorf_get(type, vec1, i);
    return p;
}

void ar_vector_scale(ar_vector_type_t type, void *vec, float s) {
    const int length = VEC_LENGTH(type);
    int i;
    for (i = 0; i < length; i++)
        ar_vectorf_set(type, vec, i, ar_vectorf_get(type, vec, i)*s);
}

#define VEC_FLOAT_DEFINE_SIMPLE_FUNCTIONS_VEC(n, type) \
    inline float vec##n##f_dot(type a, type b) { \
        float p = 0.0f; \
        int i; \
        for (i = 0; i < n; i++) \
            p += vecf_get_at(n, &a, i)*vecf_get_at(n, &b, i); \
        return p; \
    } \
    inline void vec##n##f_scale(type *r, type v, float s) { \
        int i; \
        for (i = 0; i < n; i++) \
            vecf_set_at(n, r, i, vecf_get_at(n, &v, i)*s); \
    } \
    inline void vec##n##f_add(type *v, type a, type b) { \
        int i; \
        for (i = 0; i < n; i++) \
            vecf_set_at(n, v, i, vecf_get_at(n, &a, i)+vecf_get_at(n, &b, i)); \
    } \
    inline void vec##n##f_sub(type *v, type a, type b) { \
        int i; \
        for (i = 0; i < n; i++) \
            vecf_set_at(n, v, i, vecf_get_at(n, &a, i)-vecf_get_at(n, &b, i)); \
    } \
    inline type vec##n##f_mul(type a, type b) { \
        type loc; \
        int i; \
        for (i = 0; i < n; i++) \
            vecf_set_at(n, &loc, i, vecf_get_at(n, &a, i)*vecf_get_at(n, &b, i)); \
        return loc; \
    } \
    inline type vec##n##f_mul_v(type a, float b) { \
        type loc; \
        int i; \
        for (i = 0; i < n; i++) \
            vecf_set_at(n, &loc, i, vecf_get_at(n, &a, i)*b); \
        return loc; \
    } \
    inline float vec##n##f_len(type v) { \
        return sqrtf(vec##n##f_dot(v, v)); \
    } \
    inline void vec##n##f_norm(type *r, type v) { \
        const float k = 1.0f/vec##n##f_len(v); \
        vec##n##f_scale(r, v, k); \
    } \
    inline int vec##n##f_cmp(type v0, type v1) { \
        int i; \
        for (i = 0; i < n; i++) { \
            if (vecf_get_at(n, &v0, i) != vecf_get_at(n, &v1, i)) \
                return i; \
        } \
        return 0; \
    }
