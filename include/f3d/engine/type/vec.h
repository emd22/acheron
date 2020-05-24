#ifndef F3D_TYPE_VEC_H
#define F3D_TYPE_VEC_H

#include <math.h>

#define VEC2F(value) (vector2f_t){value, value};
#define VEC3F(value) (vector3f_t){value, value, value};

#define VEC2F_VALUE(vec) (vector2f_t){vec.x, vec.y}
#define VEC3F_VALUE(vec) (vector3f_t){vec.x, vec.y, vec.z}
#define VEC4F_VALUE(vec) (vector4f_t){vec.x, vec.y, vec.z, vec.w}

enum {
    TYPE_VEC2F = 2,
    TYPE_VEC3F,
    TYPE_VEC4F,
};

enum {
    TYPE_VEC2 = 2,
    TYPE_VEC3,
    TYPE_VEC4,
};

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

// float
typedef struct {
    float x, y, z, w;
} vector4f_t;

typedef struct {
    float x, y, z;
} vector3f_t;

typedef struct {
    float x, y;
} vector2f_t;

// int type
typedef struct {
    int x, y, z, w;
} vector4_t;

typedef struct {
    int x, y, z;
} vector3_t;

void vecf_set_at(int size, void *vector, int index, float value);
float vecf_get_at(int size, void *vector, int index);

// vec2f_*
VEC_FLOAT_DEFINE_SIMPLE_FUNCTIONS_VEC(2, vector2f_t)
// vec3f_*
VEC_FLOAT_DEFINE_SIMPLE_FUNCTIONS_VEC(3, vector3f_t)
// vec4f_*
VEC_FLOAT_DEFINE_SIMPLE_FUNCTIONS_VEC(4, vector4f_t)

#endif
