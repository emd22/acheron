#include <gl/camera.h>
#include <gl/window.h>
#include <gl/log.h>
#include <gl/math.h>
#include <gl/types.h>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define RADIAN_MAX 6.2831

camera_t *selected_camera = NULL;

camera_t camera_new(void) {
    camera_t camera;
    camera.rotation = (vector3f_t){0.0f, 0.0f, 0.0f};
    camera.position = (vector3f_t){0.0f, 0.0f, 0.0f};
    camera.direction = (vector3f_t){0.0f, 0.0f, 0.0f};
    return camera;
}

void camera_clamp_rotation(camera_t *camera) {
    // Rotation X
    if (camera->rotation.x > 6.2831f/*360deg in radians*/)
        camera->rotation.x = 0.0f;
    else if (camera->rotation.x < 0)
        camera->rotation.x = 6.2831f;
    // Rotation Y
    if (camera->rotation.y > 6.2831f)
        camera->rotation.y = 0.0f;
    else if (camera->rotation.y < 0)
        camera->rotation.y = 6.2831f;
    // Rotation Z
    if (camera->rotation.z > 6.2831f)
        camera->rotation.z = 0.0f;
    else if (camera->rotation.z < 0)
        camera->rotation.z = 6.2831f;
}

void camera_move(camera_t *camera, int direction, float speed) {
    float add = 0;
    if (direction == CAMERA_DIRECTION_LEFT) {
        add = 90;
        direction = -1;
    }
    else if (direction == CAMERA_DIRECTION_RIGHT) {
        add = 90;
        direction = 1;
    }
    float ny = (camera->rotation.y+math_deg_to_rad(add));
    camera->position.x -= sin(ny)*direction*speed;
    camera->position.z += cos(ny)*direction*speed;
    (void)add;
}

void camera_select(camera_t *camera) {
    window_t *window = window_get_default();
    math_perspective(&camera->mat_projection, 95, (float)window->width/(float)window->height, 0.1f, 100.0f);
    selected_camera = camera;
}
 
void camera_update(camera_t *camera, unsigned shaderid) {
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
    
    mat4_set(
        &model,
        (float []){
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    );
    //mat4_translate(&view, (vector3f_t){});
    projection = mat4_rotate_x(camera->mat_projection, camera->rotation.x);
    projection = mat4_rotate_y(projection, -camera->rotation.y);
    mat4_translate(&view, (vector3f_t){camera->position.x, camera->position.y, camera->position.z-6});
    view.val[12] += camera->direction.x;
    view.val[13] += camera->direction.y;
    view.val[14] += camera->direction.z;
    //mv += 0.1;
    //mat4_translate(&model, (vector3f_t){sinf(mv), 0, 0});
    mat4_t tmp = mat4_mul(projection, model);
    mat4_t mvp = mat4_mul(tmp, view);

    unsigned matrixid = glGetUniformLocation(shaderid, "MVP");
    glUniformMatrix4fv(matrixid, 1, GL_FALSE, mvp.val);
}
