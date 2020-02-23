#include <gl/type/vec.h>

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
