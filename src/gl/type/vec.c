#include <gl/type/vec.h>
#include <gl/c3d.h>


vector3f_t vec3f_sub_vec3f(vector3f_t vec0, vector3f_t vec1) {
    vector3f_t res;
    res.x = vec0.x-vec1.x;
    res.y = vec0.y-vec1.y;
    res.z = vec0.z-vec1.z;
    return res;
}

int vecf_set_at(void *vector, int type, int index, float value) {
    if (type == TYPE_VEC2F) {
        vector2f_t *vec = (vector2f_t *)vector;
        if (index == 0) vec->x = value;
        else if (index == 1) vec->x = value;
        else return C3D_FAILURE;
        return C3D_SUCCESS;
    }
    else if (type == TYPE_VEC3F) {
        vector3f_t *vec = (vector3f_t *)vector;
        if (index == 0) vec->x = value;
        else if (index == 1) vec->y = value;
        else if (index == 2) vec->z = value;
        else return C3D_FAILURE;
    }
    else if (type == TYPE_VEC4F) {
        vector4f_t *vec = (vector4f_t *)vector;
        if (index == 0) vec->x = value;
        else if (index == 1) vec->y = value;
        else if (index == 2) vec->z = value;
        else if (index == 3) vec->w = value;
        else return C3D_FAILURE;
    }
    return C3D_FAILURE;
}
float vecf_get_at(void *vector, int type, int index) {
    if (type == TYPE_VEC2F) {
        vector2f_t *vec = (vector2f_t *)vector;
        if (index == 0)
            return vec->x;
        else if (index == 1)
            return vec->y;
    }
    else if (type == TYPE_VEC3F) {
        vector3f_t *vec = (vector3f_t *)vector;
        if (index == 0)
            return vec->x;
        else if (index == 1)
            return vec->y;
        else if (index == 2)
            return vec->z;
    }
    else if (type == TYPE_VEC4F) {
        vector4f_t *vec = (vector4f_t *)vector;
        if (index == 0)
            return vec->x;
        else if (index == 1)
            return vec->y;
        else if (index == 2)
            return vec->z;
        else if (index == 3)
            return vec->w;
    }
    return C3D_FAILURE;
}

