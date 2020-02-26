#include <gl/type/vec.h>

vector3f_t vec3f_sub_vec3f(vector3f_t vec0, vector3f_t vec1) {
    vector3f_t res;
    res.x = vec0.x-vec1.x;
    res.y = vec0.y-vec1.y;
    res.z = vec0.z-vec1.z;
    return res;
}

float vec4f_mul_inner(vector4f_t a, vector4f_t b) {
    float p = 0.0f;
    int i;
    for (i = 0; i < 4; i++)
        p += vec4f_get(&b, i)*vec4f_get(&a, i);
    return p;
}

void vec4f_set(vector4f_t *vec, int index, float value) {
    if (index == 0)
        vec->x = value;
    else if (index == 1)
        vec->y = value;
    else if (index == 2)
        vec->z = value;
    else if (index == 3)
        vec->w = value;
}

float vec4f_get(vector4f_t *vec, int index) {
    if (index == 0)
        return vec->x;
    else if (index == 1)
        return vec->y;
    else if (index == 2)
        return vec->z;
    else if (index == 3)
        return vec->w;
    return -1;
}
