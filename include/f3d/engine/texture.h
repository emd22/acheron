#ifndef F3D_TEXTURE_H
#define F3D_TEXTURE_H

#include <f3d/engine/image.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define TEXTURE_TYPE_RGB     GL_RGB
#define TEXTURE_TYPE_RGBA    GL_RGBA
#define TEXTURE_TYPE_DEPTH   GL_DEPTH_COMPONENT
#define TEXTURE_TYPE_DEPTH16 GL_DEPTH_COMPONENT16

typedef struct {
    unsigned id;
    int bpp;
    // type of data stored e.g RGB, RGBA, etc.
    int data_type;
    int draw_type;
    int index;
    image_t image;
} texture_t;

texture_t *texture_new(void);
void texture_init(texture_t *texture);
texture_t *texture_load(texture_t *texture, const char *path, int type);
void texture_set_data(texture_t *texture, int width, int height, int bpp, unsigned char *data);
void texture_destroy(texture_t *tex);
void textures_cleanup(void);

#endif
