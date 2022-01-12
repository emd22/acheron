#include <acheron/engine/camera/cm_perspective.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/types.h>

#include <acheron/engine/core/memory/mm_alloc.h>

#include <acheron/engine/acheron.h>
#include <string.h>

static void *perspective_reload(ar_camera_t *camera);
static void *perspective_update(ar_camera_t *camera);
static void *perspective_destroy(ar_camera_t *camera);

ar_camera_t ar_camera_perspective_new(void) {
    ar_camera_t camera = ar_camera_new();
    // this gets free'd at destruction of ar_camera_t object.
    camera.info = ar_memory_alloc(sizeof(ar_camera_perspective_info_t));
    ar_camera_perspective_info_t *info = camera.info;

    info->near = 0.1f;
    info->far = 200.0f;
    info->fov = 100.0f;

    // setup functions
    camera.reload = &perspective_reload;
    camera.update = &perspective_update;
    camera.destroy = &perspective_destroy;

    camera.reload(&camera);
    return camera;
}

int ar_camera_fov(ar_camera_t *camera, int value) {
    ar_camera_perspective_info_t *info = camera->info;
    if (value != AR_VALUE_QUERY)
        info->fov = value;
    // recalculate perspective matrix with new fov
    camera->reload(camera);
    return info->fov;
}

static void *perspective_update(ar_camera_t *camera) {
    camera->direction = (ar_vector3f_t){
        sinf(camera->rotation.x),
        camera->rotation.y,
        cosf(camera->rotation.x)
    };
    return NULL;
}

static void *perspective_reload(ar_camera_t *camera) {
    ar_instance_t *inst = ar_instance_get_selected();
    const float aspect = (float)inst->window->width/(float)inst->window->height;

    ar_camera_perspective_info_t *info = camera->info;
    
    ar_math_perspective(
        &camera->projection,
        info->fov, aspect,
        info->near, info->far
    );

    return NULL;
}

static void *perspective_destroy(ar_camera_t *camera) {
    if (camera) {
        ar_memory_free(camera->info);
    }
    return NULL;
}
