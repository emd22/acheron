#include <acheron/engine/camera/cm_ortho.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/types.h>

#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_log.h>
#include <string.h>

static void *ortho_update(ar_camera_t *camera);
static void *ortho_reload(ar_camera_t *camera);

ar_camera_t ar_camera_ortho_new(void) {
    ar_camera_t camera = ar_camera_new();
    // this gets free'd at destruction of ar_camera_t object.
    camera.info = ar_memory_alloc(sizeof(ar_camera_ortho_info_t));

    ar_camera_ortho_info_t *info = camera.info;

    info->left = 0.0f;
    info->right = 10.0f;
    info->bottom = 0.0f;
    info->top = 10.0f;

    info->near = 0.1f;
    info->far = 50.0f;

    // setup functions
    camera.reload = &ortho_reload;
    camera.update = &ortho_update;

    camera.reload(&camera);

    return camera;
}

static void *ortho_update(ar_camera_t *camera) {
    camera->direction = (ar_vector3f_t){
        sinf(camera->rotation.x),
        camera->rotation.y,
        cosf(camera->rotation.x)
    };
    return NULL;
}

static void *ortho_reload(ar_camera_t *camera) {
    ar_camera_ortho_info_t *info = camera->info;
    ar_math_ortho(
        &camera->projection, 
        info->left,   info->right, 
        info->bottom, info->top,
        info->near,   info->far
    );

    return NULL;
}
