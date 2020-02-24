#include <gl/type/vec.h>

vector3f_t vec3f_sub_vec3f(vector3f_t vec0, vector3f_t vec1) {
    vector3f_t res;
    res.x = vec0.x-vec1.x;
    res.y = vec0.y-vec1.y;
    res.z = vec0.z-vec1.z;
    return res;
}

float vec4f_get(vector4f_t *vec, int index) {
    if (index == 0)
        return vec->w;
    else if (index == 1)
        return vec->x;
    else if (index == 2)
        return vec->y;
    else if (index == 3)
        return vec->z;
    return -1;
}
