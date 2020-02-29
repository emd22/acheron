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
    return (degrees*C3D_PI/180);
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
    //floatset(projection.val, 1, 16);
    //mat4_t view;
    (void)camera;
    static float angle = 0.0f;
    angle += 0.1;
    if (angle >= 360)
        angle = 0.1;
    mat4_t model, view, projection;
    mat4_set(
        &view, 
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        }
    );
    //mat4_translate(&view, (vector3f_t){4, 3, 3});
    
    math_perspective(&projection, 95, 4.0f/3.0f, 0.1f, 100.0f);

    mat4_set(
        &model,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    mat4_translate(&view, (vector3f_t){0, 0, -7});
    view = mat4_rotate_x(view, angle);
    mat4_t tmp = mat4_mul(projection, view);
    mat4_t mvp = mat4_mul(tmp, model);

    unsigned matrixid = glGetUniformLocation(shaderid, "MVP");
    glUniformMatrix4fv(matrixid, 1, GL_FALSE, mvp.val);
}
