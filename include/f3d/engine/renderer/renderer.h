#ifndef AR_RENDERER_H
#define AR_RENDERER_H

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
void ar_renderer_destroy(ar_renderer_instance_t *instance);

void ar_renderer_enable(ar_renderer_enable_t code);
void ar_renderer_disable(ar_renderer_enable_t code);

void ar_renderer_opengl_init(void);

#endif
