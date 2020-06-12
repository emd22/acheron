#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/core/log.h>

#include <math.h>
#include <string.h>

#define AR_PI 3.1415926535

double math_deg_to_rad(double degrees) {
    return degrees*(AR_PI/180);
}

double math_rad_to_deg(double rad) {
    return rad*(180/AR_PI);
}


ar_vector3f_t math_cross(ar_vector3f_t vec0, ar_vector3f_t vec1) {
    ar_vector3f_t res;
    res.x = vec0.y*vec1.z-vec0.z*vec1.y;
    res.y = vec0.z*vec1.x-vec0.x*vec1.z;
    res.z = vec0.x*vec1.y-vec0.y*vec1.x;
    return res;
}

ar_vector3f_t normalize(ar_vector3f_t vec) {
    ar_vector3f_t res = {vec.x, vec.y, vec.z};
    float n = (vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z);
    //if (n > 0) {
    float factor = 1/sqrt(n);
    res.x *= factor;
    res.y *= factor;
    res.z *= factor;
    //}
    
    return res;
}

float math_dot(ar_vector3f_t vec0, ar_vector3f_t vec1) {
    return (vec0.x*vec1.x+vec0.y*vec1.y+vec0.z*vec1.z);
}

void math_ortho(mat4_t *mat, float l, float r, float b, float t, float n, float f) {
    const float rsl = r-l, ral = r+l;
    const float tsb = t-b, tab = t+b;
    const float fsn = f-n, fan = f+n;
    mat4_set(
        mat, 
        (float []){
            2.0f/rsl, 0.0f,      0.0f,     0.0f,
            0.0f,     2.0f/tsb,  0.0f,     0.0f,
            0.0f,     0.0f,     -2.0f/fsn, 0.0f,
            -ral/rsl, -tab/tsb,  -fan/fsn, 1.0f
        }
    );
}

mat4_t math_lookat(ar_vector3f_t from, ar_vector3f_t to, ar_vector3f_t upvec) {
    ar_vector3f_t forward, right, up;
    ar_vector_sub(AR_VEC3F, &to, &from, &forward);
    forward = normalize(forward);
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
    mat4_translate_in_place(&res, (ar_vector3f_t){-from.x, -from.y, -from.z});
    return res; 
}

// https://www.khronos.org/opengl/wiki/GluPerspective_code

void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar) {
    float a = 1.0f/tan(fovy/2.0f);
   // const float zn = -((zfar+znear)/(zfar-znear));
   // const float zn2 = -((2.0f*zfar*znear)/(zfar-znear));
    
    const float rangeinf = 1.0f/(znear-zfar);
    const float zn = (znear+zfar)*rangeinf;
    const float zn2 = znear*zfar*rangeinf*2;
    
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

