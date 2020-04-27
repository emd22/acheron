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
} framebuffer_t;

extern framebuffer_t *default_framebuffer;

framebuffer_t framebuffer_new(int width, int height, int attachment, bool depth_buffer);
void framebuffer_generate_texture(framebuffer_t *fb, int draw_type, int data_type, int type_size);
void framebuffer_bind(framebuffer_t *fb);
void framebuffer_texture(framebuffer_t *fb, int attachment);
void framebuffer_destroy(framebuffer_t *fb);

#endif
