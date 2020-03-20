#include <f3d/engine/camera.h>
#include <f3d/engine/window.h>
#include <f3d/engine/log.h>
#include <f3d/engine/math.h>
#include <f3d/engine/types.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/time.h>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define RADIAN_MAX 6.2831

camera_t *selected_camera = NULL;

camera_t camera_new(void) {
    camera_t camera;
    memset(&camera, 0, sizeof(camera_t));
    camera.fov = 65;
    camera.direction = (vector3f_t){0, 0, 0};
    return camera;
}

void camera_clamp_rotation(camera_t *camera) {
    // Rotation X
    if (camera->rotation.x > 6.2831f)
        camera->rotation.x = 0.0f;
    else if (camera->rotation.x < 0)
        camera->rotation.x = 6.2831f;
    // Rotation Y
    if (camera->rotation.y > F3D_PI)
        camera->rotation.y = F3D_PI;
    else if (camera->rotation.y < -math_deg_to_rad(60))
        camera->rotation.y = -math_deg_to_rad(60);
}

void camera_move(camera_t *camera, int direction) {
    switch (direction) {
        case CAMERA_FORWARD:
            camera->direction.y = 0;
            camera->direction = vec3f_mul_v(camera->direction, camera->move_speed);
            camera->direction = vec3f_mul_v(camera->direction, delta_time);
            vec3f_add(&(camera->position), camera->position, (camera->direction));
            break;
        case CAMERA_BACKWARD:
            camera->direction.y = 0;
            camera->direction = vec3f_mul_v(camera->direction, camera->move_speed);
            camera->direction = vec3f_mul_v(camera->direction, delta_time);
            vec3f_sub(&(camera->position), camera->position, (camera->direction));
            break;
        case CAMERA_LEFT:
            camera->right = vec3f_mul_v(camera->right, camera->move_speed);
            camera->right = vec3f_mul_v(camera->right, delta_time);
            vec3f_sub(&(camera->position), camera->position, (camera->right));
            break;
        case CAMERA_RIGHT:
            camera->right = vec3f_mul_v(camera->right, camera->move_speed);
            camera->right = vec3f_mul_v(camera->right, delta_time);
            vec3f_add(&(camera->position), camera->position, (camera->right));
            break;
        default:
            return;
    }    
}

void camera_select(camera_t *camera) {
    math_perspective(
        &camera->mat_projection, 
        math_deg_to_rad(camera->fov), 
        (float)default_window->width/(float)default_window->height,
        0.1f,
        100.0f
    );
    selected_camera = camera;
}
  
void camera_update(camera_t *camera, unsigned shaderid) {
    mat4_t view, projection = camera->mat_projection;
    (void)shaderid;
    camera_clamp_rotation(camera);

    camera->direction = (vector3f_t){
        cos(camera->rotation.y)*sin(camera->rotation.x),
        sin(camera->rotation.y),
        cos(camera->rotation.y)*cos(camera->rotation.x)
    };
    camera->right = (vector3f_t){
        sin(camera->rotation.x-3.14f/2.0f),
        0,
        cos(camera->rotation.x-3.14f/2.0f)
    };
    camera->up = math_cross(camera->right, camera->direction);
    //projection = mat4_rotate_x(camera->mat_projection, camera->rotation.x);
    //projection = mat4_rotate_y(projection, -camera->rotation.y);
    vector3f_t lookto;
    lookto.x = camera->position.x+camera->direction.x;
    lookto.y = camera->position.y+camera->direction.y;
    lookto.z = camera->position.z+camera->direction.z;
    view = math_lookat(camera->position, lookto, camera->up);
    //mat4_set(&view, MAT4_IDENTITY);
    //log_msg(LOG_INFO, "DIR: (%f,%f,%f)\n", lookto.x, lookto.y, lookto.z);

    camera->vp_mat = mat4_mul(projection, view);
    //mat4_print(&camera->vp_mat);
}
