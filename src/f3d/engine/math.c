#include <f3d/engine/math.h>
#include <f3d/engine/log.h>

#include <math.h>
#include <string.h>

vector3f_t math_cross(vector3f_t vec0, vector3f_t vec1) {
    vector3f_t res;
    res.x = vec0.y*vec1.z-vec0.z*vec1.y;
    res.y = vec0.z*vec1.x-vec0.x*vec1.z;
    res.z = vec0.x*vec1.y-vec0.y*vec1.x;
    return res;
}

vector3f_t normalize(vector3f_t vec) {
    vector3f_t res = {vec.x, vec.y, vec.z};
    float n = (vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z);
    if (n > 0) {
        float factor = 1/sqrt(n);
        res.x *= factor;
        res.y *= factor;
        res.z *= factor;
    }
    
    return res;
}

float math_dot(vector3f_t vec0, vector3f_t vec1) {
    return (vec0.x*vec1.x+vec0.y*vec1.y+vec0.z*vec1.z);
}

mat4_t math_lookat(vector3f_t from, vector3f_t to, vector3f_t upvec) {
    vector3f_t forward, right, up;
    forward = normalize(vec3f_sub_vec3f(to, from));
    right = normalize(math_cross(forward, upvec));
    up = math_cross(right, forward);
    mat4_t res;
    mat4_set(
        &res, 
        (float []){
            right.x, up.x, -forward.x, 0.0f,
            right.y, up.y, -forward.y, 0.0f,
            right.z, up.z, -forward.z, 0.0f,
            0.0f,    0.0f,  0.0f,      1.0f
        }
    );
    mat4_translate_in_place(&res, (vector3f_t){-from.x, -from.y, -from.z});
    return res; 
}

// https://www.khronos.org/opengl/wiki/GluPerspective_code

void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar) {
    float a = 1.0f/tan(fovy/2.0f);
    const float zn = -((zfar+znear)/(zfar-znear));
    const float zn2 = -((2.0f*zfar*znear)/(zfar-znear));
    mat4_set(
        mat, 
        (float []){
            a/aspect, 0, 0,   0,
            0,        a, 0,   0,
            0,        0, zn, -1,
            0,        0, zn2, 0,
        }
    );
}

