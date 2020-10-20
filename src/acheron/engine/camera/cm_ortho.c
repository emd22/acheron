#include <acheron/engine/camera/cm_ortho.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/types.h>

#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_log.h>
#include <string.h>

static void *ortho_update(ar_camera_t *camera, void *arg);
static void *ortho_reload(ar_camera_t *camera, void *arg);

ar_camera_ortho_t ar_camera_ortho_new(void) {
    ar_camera_ortho_t camera;
    memset(&camera, 0, sizeof(camera));
    camera.camera = ar_camera_new();

    ar_instance_t *inst = ar_instance_get_selected();
    camera.left = 0.0f;
    camera.right = 10;
    (void)inst;
    camera.bottom = 0.0f;
    camera.top = 10.0f;

    camera.near = 0.1f;
    camera.far = 50.0f;

    camera.camera.reload = &ortho_reload;
    camera.camera.update = &ortho_update;
    
    camera.camera.reload(&camera.camera, &camera);
    return camera;
}

static void *ortho_update(ar_camera_t *camera, void *arg) {
    (void)arg;
    (void)camera;
    camera->direction = (ar_vector3f_t){
        sinf(camera->rotation.x),
        camera->rotation.y,
        cosf(camera->rotation.x)
    };
    return NULL;
}

static void *ortho_reload(ar_camera_t *camera, void *arg) {
    ar_camera_ortho_t *ocam = (ar_camera_ortho_t *)arg;

    ar_math_ortho(
        &camera->projection, 
        ocam->left, ocam->right, 
        ocam->bottom, ocam->top,
        ocam->near, ocam->far
    );

    return NULL;
}
