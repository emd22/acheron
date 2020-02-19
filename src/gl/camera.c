#include <gl/camera.h>
#include <gl/window.h>
#include <gl/log.h>
#include <gl/math.h>

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

void camera_perspective(float fovy, float aspect, float znear, float zfar) {
    mat4_t mat;
    math_perspective_m4(&mat, fovy, aspect, znear, zfar);
    glLoadMatrixf(mat.val);
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
    //log_msg(LOG_INFO, "%.02f %.02f\n", camera->z, camera->x);
    camera_update(camera);
}

void camera_select(camera_t *camera) {
    selected_camera = camera;
    camera_update(camera);
}
 
void camera_update(camera_t *camera) {
    glLoadIdentity();
    const int   eye_angle = 85;
    const float cam_len   = 1.0f;
    const float dim_ratio = (float)640/(float)480;
    camera_perspective(eye_angle, dim_ratio, cam_len, 1024.0);
    glRotatef(-camera->pitch, 1, 0, 0);
    glRotatef(-camera->yaw,   0, 1, 0);
    glTranslatef(-camera->x, -camera->y, -camera->z);
}
