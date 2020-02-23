#include <gl/math.h>

#include <math.h>
#include <string.h>

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

    (void)mat;
    (void)temp;
    (void)temp2;
    (void)temp3;
    (void)temp4;
    /*float *data = mat->val;
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
    data[15] = 0;*/
}
