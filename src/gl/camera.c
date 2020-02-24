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
    const float fovy = 45;
    const float cam_len   = 0.1f;
    const float aspect = (float)640/(float)480;
    //floatset(projection.val, 1, 16);
    //mat4_t view;
    (void)camera;
    static float mv = 0;
    static float dir = 0.1;
    mv += dir;
    if (mv >= 5 || mv <= -5) {
        dir = -dir;
    }
    mat4_t model, view, projection;//, projection;
    view = math_lookat(
        (vector3f_t){4, 3, 3},
        (vector3f_t){0, 0, 0},
        (vector3f_t){0, -1, 0}
    );
    //mat4_t projection = camera_perspective(eye_angle, dim_ratio, cam_len, 100.0);
    math_perspective(&projection, fovy, aspect, cam_len, 100);
    
    mat4_set(
        &model,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    //static float angle = 0;
    //rotz += 1.0f;
    //angle += 1.0f;
    //if (angle > 360)
    //    angle = 1;
    mat4_t tmp = mat4_mul(projection, view);
    mat4_t mvp = mat4_mul(tmp, model);
    
    //mat4_rotate(&mvp, (vector3f_t){degrees_to_radians(1), degrees_to_radians(1), degrees_to_radians(1)});
    //mvp = mat4_mul_vec4(mvp, (vector4f_t){0.2, 0.2, 0.2, 1});

    unsigned matrixid = glGetUniformLocation(shaderid, "MVP");
    glUniformMatrix4fv(matrixid, 1, GL_FALSE, mvp.val);
    /*glLoadIdentity();
    
    glRotatef(-camera->pitch, 1, 0, 0);
    glRotatef(-camera->yaw,   0, 1, 0);
    glTranslatef(-camera->x, -camera->y, -camera->z);*/
}
