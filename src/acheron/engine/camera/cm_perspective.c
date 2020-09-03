#include <acheron/engine/camera/cm_perspective.h>
#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/types.h>

#include <acheron/engine/acheron.h>
#include <string.h>

static void *perspective_reload(ar_camera_t *camera, void *arg);
static void *perspective_update(ar_camera_t *camera, void *arg);

ar_camera_perspective_t ar_camera_perspective_new(void) {
    ar_camera_perspective_t camera;
    memset(&camera, 0, sizeof(camera));
    camera.camera = ar_camera_new();
    camera.near = 0.1f;
    camera.far = 200.0f;
    camera.fov = 80.0f;
    camera.camera.reload = &perspective_reload;
    camera.camera.update = &perspective_update;
    
    camera.camera.reload(&camera.camera, &camera);
    return camera;
}

static void *perspective_update(ar_camera_t *camera, void *arg) {
    (void)arg;
    camera->direction = (ar_vector3f_t){
        sinf(camera->rotation.x),
        camera->rotation.y,
        cosf(camera->rotation.x)
    };
    return NULL;
}

static void *perspective_reload(ar_camera_t *camera, void *arg) {
    ar_camera_perspective_t *pcam = (ar_camera_perspective_t *)arg;

    ar_instance_t *inst = ar_instance_get_selected();
    const float aspect = (float)inst->window->width/(float)inst->window->height;
    const float fov = ar_math_deg_to_rad(pcam->fov);
    
    ar_math_perspective(
        &camera->projection,
        fov, aspect,
        pcam->near, pcam->far
    );

    return NULL;
}
