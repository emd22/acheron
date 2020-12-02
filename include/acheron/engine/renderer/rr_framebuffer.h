#ifndef AR_RR_FRAMEBUFFER_H
#define AR_RR_FRAMEBUFFER_H

#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/core/cr_window.h>

typedef enum {
    AR_FRAMEBUFFER_DRAW,
    AR_FRAMEBUFFER_READ,
    AR_FRAMEBUFFER,
} ar_framebuffer_bind_type_t;

typedef enum {
    AR_FRAMEBUFFER_COLOR0,
} ar_framebuffer_attachment_type_t;

typedef struct {
    ar_texture_id_t id;
    ar_texture_t *texture;

    int x, y;
    int width, height;
} ar_framebuffer_t;

ar_framebuffer_t ar_framebuffer_new(unsigned width, unsigned height);
void ar_framebuffer_bind(ar_framebuffer_t *fb);
void ar_framebuffer_to_texture(ar_framebuffer_t *fb, ar_texture_t *texture, ar_framebuffer_attachment_type_t attachment);
void ar_framebuffer_destroy(ar_framebuffer_t *fb);

#endif