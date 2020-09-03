#include <acheron/engine/rendering/camera.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/core/cr_window.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/acheron.h>
#include <acheron/engine/types.h>
#include <acheron/engine/rendering/render.h>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define RADIAN_MAX 6.2831

typedef int isoerr_t;

/*
camera_t *selected_camera = NULL;
camera_t camera_new(camera_type_t type) {
    camera_t camera;
    memset(&camera, 0, sizeof(camera_t));
    camera.direction = (ar_vector3f_t){0, 0, 0};
    camera.rotation = (ar_vector3f_t){0, 0, 0};
    camera.type = type;
    
    // NOTE: FOV is stored in degrees, converted to radians when creating
    // perspective matrix
    camera.fov = 75;
    camera.move_mul.x = 1.0f;
    camera.move_mul.z = 1.0f;
    return camera;
}

void camera_clamp_rotation(camera_t *camera) {
    const float y_max = math_deg_to_rad(90);
    const float y_min = -y_max;
    
    // 2*PI
    const float x_limit = 6.2831f;
    
    // Rotation X
    if (camera->rotation.x > x_limit)
        camera->rotation.x = 0;
    else if (camera->rotation.x < 0)
        camera->rotation.x = x_limit;
    // Rotation Y
    if (camera->rotation.y > y_max)
        camera->rotation.y = y_max;
    else if (camera->rotation.y < y_min)
        camera->rotation.y = y_min;
}

void camera_move(camera_t *camera, int direction) {
    const float speed = camera->move_speed;
    switch (direction) {
        case CAMERA_FORWARD:
            camera->direction.y = 0;
            ar_vector_mul_value(AR_VEC3F, &camera->direction, delta_time, &camera->direction);
            ar_vector_mul_value(AR_VEC3F, &camera->direction, speed*camera->move_mul.z, &camera->direction);
            ar_vector_add(AR_VEC3F, &camera->position, &camera->direction, &camera->position);
            break;
        case CAMERA_BACKWARD:
            camera->direction.y = 0;
            ar_vector_mul_value(AR_VEC3F, &camera->direction, delta_time, &camera->direction);
            ar_vector_mul_value(AR_VEC3F, &camera->direction, speed*camera->move_mul.z, &camera->direction);
            ar_vector_sub(AR_VEC3F, &camera->position, &camera->direction, &camera->position);
            break;
        case CAMERA_RIGHT:
            ar_vector_mul_value(AR_VEC3F, &camera->right, delta_time, &camera->right);
            ar_vector_mul_value(AR_VEC3F, &camera->right, speed*camera->move_mul.x, &camera->right);
            ar_vector_add(AR_VEC3F, &camera->position, &camera->right, &camera->position);
            break;
        case CAMERA_LEFT:
            ar_vector_mul_value(AR_VEC3F, &camera->right, delta_time, &camera->right);
            ar_vector_mul_value(AR_VEC3F, &camera->right, speed*camera->move_mul.x, &camera->right);
            ar_vector_sub(AR_VEC3F, &camera->position, &camera->right, &camera->position);
            break;
        default:
            return;
    }    
}

void generate_perspective_matrix(camera_t *camera) {
    ar_window_t *default_window = ar_instance_get_selected()->window;
    const float aspect = (float)default_window->width/(float)default_window->height;
    ar_log(AR_LOG_INFO, "Aspect ratio is %.02f\n", aspect);
    math_perspective(
        &camera->mat_projection,
        math_deg_to_rad(camera->fov),
        aspect,
        0.01f,
        1000.0f
    );
}

void generate_ortho_matrix(camera_t *camera) {
    math_ortho(&camera->mat_projection, -10, 10, -10, 10, -10, 10);
}

void camera_select(camera_t *camera) {
    // generate projection matrix
    if (camera->type == CAMERA_PERSPECTIVE)
        generate_perspective_matrix(camera);
    else if (camera->type == CAMERA_ORTHOGRAPHIC)
        generate_ortho_matrix(camera);
        
    selected_camera = camera;
}
  
void camera_update(camera_t *camera) {
    // clamp rotation to avoid camera flipping on y axis or values overflowing
    camera_clamp_rotation(camera);

    // direction camera is facing
    camera->direction = (ar_vector3f_t){
        sinf(camera->rotation.x),
        (camera->rotation.y),
        cosf(camera->rotation.x)
    };
    
    camera->right = (ar_vector3f_t){
        sinf(camera->rotation.x-3.14f/2.0f),
        0,
        cosf(camera->rotation.x-3.14f/2.0f)
    };
    
    // the camera should always be facing upwards
    //const vector3f_t up = (vector3f_t){0, 1, 0};
    camera->up = (ar_vector3f_t){0, 1, 0};
    
    ar_vector3f_t lookto;
    lookto.x = camera->position.x+camera->direction.x;
    lookto.y = camera->position.y+camera->direction.y;
    lookto.z = camera->position.z+camera->direction.z;
    
    camera->mat_view = math_lookat(camera->position, lookto, camera->up);
    
    ar_shader_set_uniform(shader_main, AR_SHADER_VEC3F, "view_pos", &selected_camera->position);
}
*/