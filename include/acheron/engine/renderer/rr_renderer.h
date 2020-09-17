#ifndef AR_RR_RENDERER_H
#define AR_RR_RENDERER_H

#include <acheron/engine/scene/sc_camera.h>

typedef enum {
    AR_RENDERER_BLEND,
    AR_RENDERER_CULL_FACE,
    AR_RENDERER_DEPTH_TEST,
    AR_RENDERER_FRAMEBUFFER_SRGB,
    AR_RENDERER_MULTISAMPLE,
    AR_RENDERER_LINE_SMOOTH,
    AR_RENDERER_POLYGON_SMOOTH,
} ar_renderer_enable_t;

typedef enum {
    AR_RENDERER_NONE,
    AR_RENDERER_OPENGL,
} ar_renderer_type_t;

typedef struct {
    ar_renderer_type_t renderer_type;
} ar_renderer_instance_t;

ar_renderer_instance_t ar_renderer_init(void);
void ar_renderer_intern_init(ar_renderer_instance_t *instance);
void ar_renderer_destroy(ar_renderer_instance_t *instance);

int ar_render_scene_objects(void *camera);
void ar_renderer_draw(ar_camera_t *camera);
const char *ar_renderer_check_error(void);

void ar_renderer_enable(ar_renderer_enable_t code);
void ar_renderer_disable(ar_renderer_enable_t code);

#endif
