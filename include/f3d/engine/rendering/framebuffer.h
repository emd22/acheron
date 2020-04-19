#ifndef F3D_FRAMEBUFFER_H
#define F3D_FRAMEBUFFER_H

#define FRAMEBUFFER_NO_TEXTURE -1

#include <f3d/engine/image/texture.h>

typedef struct {
    unsigned fbo;
    //long texture_id;
    texture_t *texture;
    int width, height;
    int bpp;
    int originx, originy;
} framebuffer_t;

framebuffer_t framebuffer_new(int width, int height, int attachment);
void framebuffer_generate_texture(framebuffer_t *fb, int draw_type, int data_type, int type_size);
void framebuffer_bind(framebuffer_t *fb);
void framebuffer_texture(framebuffer_t *fb, int attachment);
void framebuffer_destroy(framebuffer_t *fb);

#endif
