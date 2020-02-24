#include <gl/math.h>

#include <math.h>
#include <string.h>

vector3f_t cross_product(vector3f_t vec0, vector3f_t vec1) {
    vector3f_t res;
    res.x = vec0.y*vec1.z-vec0.z*vec1.y;
    res.y = vec0.x*vec1.z-vec0.z*vec1.x;
    res.z = vec0.x*vec1.y-vec0.y*vec1.x;
    return res;
}

vector3f_t normalize(vector3f_t vec) {
    vector3f_t res;
    float w = sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
    res.x = vec.x/w;
    res.y = vec.y/w;
    res.z = vec.z/w;
    return res;
}

mat4_t math_lookat(vector3f_t from, vector3f_t to, vector3f_t upvec) {
    vector3f_t forward, right, up;
    
    forward = normalize(vec3f_sub_vec3f(from, to));
    right = cross_product(normalize(upvec), forward);
    up = cross_product(forward, right);
    
    mat4_t res;
    mat4_set(
        &res, 
        (float []){
            right.x, up.x, -forward.x, 0.0f,
            right.y, up.x, -forward.x, 0.0f,
            right.z, up.x, -forward.x, 0.0f,
            from.x,  from.y, from.z,   1.0f
        }
    );
    return res; 
}

// https://www.khronos.org/opengl/wiki/GluPerspective_code

void math_perspective(mat4_t *mat, float fovy, float aspect, float znear, float zfar) {
    float ymax = znear*tanf(fovy*M_PI/360);
    float xmax = ymax*aspect;
    math_frustum_mat4(mat, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void math_frustum_mat4(
    mat4_t *mat, float left, float right, 
    float bottom, float top, 
    float znear, float zfar)
{
    // don't blame me for this
    float temp = 2.0f*znear;
    float temp2 = right-left;
    float temp3 = top-bottom;
    float temp4 = zfar-znear;

    float *data = mat->val;
    data[0] = temp/temp2;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = temp/temp3;
    data[6] = 0;
    data[7] = 0;
    data[8] = (right+left)/temp2;
    data[9] = (top+bottom)/temp3;
    data[10] = (-zfar-znear)/temp4;
    data[11] = -1;
    data[12] = 0;
    data[13] = 0;
    data[14] = (-temp*zfar)/temp4;
    data[15] = 1;
}
