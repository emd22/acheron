#ifndef F3D_FRAMEBUFFER_H
#define F3D_FRAMEBUFFER_H

#define FRAMEBUFFER_NO_TEXTURE -1

#include <stdbool.h>
#include <f3d/engine/image/texture.h>

typedef struct {
    unsigned fbo;
    unsigned depth_buffer;
    
    unsigned texture_target; // GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP
    
    texture_t *texture;
    int width, height;
    int bpp;
    int originx, originy;
} ar_framebuffer_t;

ar_framebuffer_t ar_framebuffer_new(int width, int height, int attachment, bool depth_buffer);
void ar_framebuffer_generate_texture(ar_framebuffer_t *fb, int draw_type, int data_type, int type_size);
void ar_framebuffer_bind(ar_framebuffer_t *fb);
void ar_framebuffer_texture(ar_framebuffer_t *fb, int attachment);
void ar_framebuffer_set_default(ar_framebuffer_t *fb);
ar_framebuffer_t *ar_framebuffer_get_default(void);
void ar_framebuffer_destroy(ar_framebuffer_t *fb);

#endif
