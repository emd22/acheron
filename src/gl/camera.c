#include <gl/camera.h>
#include <gl/window.h>
#include <gl/log.h>
#include <gl/math.h>
#include <gl/types.h>
#include <gl/shader.h>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define RADIAN_MAX 6.2831

camera_t *selected_camera = NULL;

camera_t camera_new(void) {
    camera_t camera;
    memset(&camera, 0, sizeof(camera_t));
    camera.fov = 45;
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

void camera_move(camera_t *camera) {
    float add = 0;

    if (camera->move.x != 0)
        add = 90;
        
    float movez = camera->move.z;// ? camera->move.z : 1;
    float movex = camera->move.x;
    if (camera->move.z == 0 && camera->move.x != 0)
        // multiply by 1 to get either -1 or 1
        movez = camera->move.x*1;
        
    if (camera->move.x == 0 && camera->move.z != 0)
        // multiply by 1 to get either -1 or 1
        movex = camera->move.z*1;
    
    float ny = (camera->rotation.y+math_deg_to_rad(add));
    camera->position.x += sin(ny)*movex*camera->move_speed;
    camera->position.z += cos(ny)*movez*camera->strafe_speed;
    (void)add;
}

void camera_select(camera_t *camera) {
    window_t *window = window_get_default();
    math_perspective(&camera->mat_projection, camera->fov, (float)window->width/(float)window->height, 0.1f, 100.0f);
    selected_camera = camera;
}
 
void camera_update(camera_t *camera, unsigned shaderid) {
    mat4_t model, view, projection;
    (void)shaderid;
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
    //mat4_translate(&model, (vector3f_t){0, 0, sin(rot += 0.1f)*2.0f});
    //model = mat4_rotate_y(model, rot += 0.01f);
    //mat4_translate(&view, (vector3f_t){});
    projection = mat4_rotate_x(camera->mat_projection, camera->rotation.x);
    projection = mat4_rotate_y(projection, -camera->rotation.y);
    mat4_translate(&view, (vector3f_t){-camera->position.x, -camera->position.y, camera->position.z});
    //mv += 0.1;
    //mat4_translate(&model, (vector3f_t){sinf(mv), 0, 0});
    camera->vp_mat = mat4_mul(projection, view);
    //mat4_t mvp = mat4_mul(camera->vp_mat, model);
    
    //shader_set_mat4(shaderid, "M", &model);
    //shader_set_mat4(shaderid, "MVP", &mvp);
}
