#include <acheron/engine/camera/cm_perspective.h>
#include <acheron/engine/types.h>

#include <acheron/engine/rendering/render.h>

#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/core/cr_time.h>
#include <acheron/engine/core/cr_log.h>

#include <math.h>
#include <string.h>

static void *camera_update_default(ar_camera_t *, void *);
static void *camera_reload_default(ar_camera_t *, void *);

ar_camera_t ar_camera_new(void) {
    ar_camera_t camera;
    memset(&camera, 0, sizeof(ar_camera_t));
    camera.update = &camera_update_default;
    camera.reload = &camera_reload_default;
    return camera;
}

static void clamp_axis(float *value, float min_r, float max_r) {
    (*value) = fmax(fmin(*value, min_r), max_r);
}

void ar_camera_clamp(ar_camera_t *camera, ar_vector3f_t axis, float min_r, float max_r) {
    if (axis.x)
        clamp_axis(&camera->rotation.x, min_r, max_r);
    else if (axis.y)
        clamp_axis(&camera->rotation.y, min_r, max_r);
    else if (axis.z)
        clamp_axis(&camera->rotation.z, min_r, max_r);
}

void ar_camera_rotate(ar_camera_t *camera, ar_vector3f_t value) {
    const float delta = ar_time_get_delta();
    camera->rotation.x += value.x*delta;
    camera->rotation.y += value.y*delta;
    camera->rotation.z += value.z*delta;
}

void ar_camera_update(ar_camera_t *camera) {
    camera->update(camera, NULL);

    camera->up = (ar_vector3f_t){0, 1, 0};
    camera->right = (ar_vector3f_t){
        sinf(camera->rotation.x-3.14f/2.0f),
        0,
        cosf(camera->rotation.x-3.14f/2.0f)
    };

    ar_vector3f_t look_position;
    ar_vector_add(AR_VEC3F, &camera->position, &camera->direction, &look_position);
    
    camera->view = ar_math_lookat(camera->position, look_position, camera->up);
    
    ar_shader_t *shader = ar_shaderman_get_render_shader();
    ar_shader_set_uniform(shader, AR_SHADER_VEC3F, "view_pos", &camera->position);
}

static void *camera_update_default(ar_camera_t *camera, void *arg) {
    (void)camera;
    (void)arg;
    return NULL;
}

static void *camera_reload_default(ar_camera_t *camera, void *arg) {
    (void)camera;
    (void)arg;
    return NULL;
}