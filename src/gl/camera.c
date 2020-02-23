#include <gl/camera.h>
#include <gl/window.h>
#include <gl/log.h>
#include <gl/math.h>
#include <gl/types.h>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

camera_t *selected_camera = NULL;

camera_t camera_new(void) {
    camera_t camera;
    memset(&camera, 0, sizeof(camera_t));
    return camera;
}

float degrees_to_radians(float degrees) {
    return (degrees*M_PI/180);
}

void camera_check(camera_t *_camera) {
    if (_camera->pitch > 90)
        _camera->pitch = 90;
    else if (_camera->pitch < -90)
        _camera->pitch = -90;
    
    if (_camera->yaw >= 360)
        _camera->yaw = 0;
    else if (_camera->yaw <= -360)
        _camera->yaw = 0;
}

mat4_t camera_perspective(float fovy, float aspect, float znear, float zfar) {
    mat4_t mat;
    math_perspective(&mat, fovy, aspect, znear, zfar);
    return mat;
}

void camera_move(camera_t *camera, int direction, float speed) {
    float add = 0;
    if (direction == CAMERA_DIRECTION_LEFT) {
        add = 90;
        direction = 1;
    }
    else if (direction == CAMERA_DIRECTION_RIGHT) {
        add = 90;
        direction = -1;
    }
    float ny = degrees_to_radians(camera->yaw+add);
    camera->z -= cos(ny)*direction*speed;
    camera->x -= sin(ny)*direction*speed;
    //log_msg(LOG_INFO, "%.02f %.02f\n", camera->z, camera->x);s
}

void floatset(float *b, int v, int amt) {
    int i;
    for (i = 0; i < amt; i++)
        b[i] = v;
}

void camera_select(camera_t *camera) {
    selected_camera = camera;
}
 
void camera_update(camera_t *camera, unsigned shaderid) {
    /*    const int   eye_angle = 85;*/
/*    const float cam_len   = 1.0f;*/
/*    const float dim_ratio = (float)640/(float)480;*/
    //mat4_t projection = camera_perspective(eye_angle, dim_ratio, cam_len, 1024.0);
    //floatset(projection.val, 1, 16);
    //mat4_t view;
    (void)camera;
    mat4_t model, view, projection;
    mat4_set(
        &view,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    mat4_set(
        &projection,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    mat4_set(
        &model,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    
    mat4_t mvp1 = mat4_mul(projection, view);
    mat4_t mvp = mat4_mul(mvp1, model);
    
    mvp = mat4_mul_vec4(mvp, (vector4f_t){1, 1, 1, 3});

    unsigned matrixid = glGetUniformLocation(shaderid, "MVP");
    glUniformMatrix4fv(matrixid, 1, GL_FALSE, mvp.val);
    /*glLoadIdentity();
    
    glRotatef(-camera->pitch, 1, 0, 0);
    glRotatef(-camera->yaw,   0, 1, 0);
    glTranslatef(-camera->x, -camera->y, -camera->z);*/
}
