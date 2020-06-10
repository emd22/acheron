#include <f3d/engine/math/vector.h>

#include <stddef.h>

typedef enum {
    AR_VEC2F,
    AR_VEC3F,
    AR_VEC4F,
} ar_vector_type_t;

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

void ar_vector_dot(ar_vector_type_t type, void *vec0, void *vec1) {


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
