#include <gl/type/vec.h>

void vec4f_get_data(vec4f_t *vec, float *data) {
    data[0] = vec->w;
    data[1] = vec->x;
    data[2] = vec->y;
    data[3] = vec->z;
}
