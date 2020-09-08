#ifndef AR_RR_TEXTURE_H
#define AR_RR_TEXTURE_H

#include <acheron/engine/image/image.h>
#include <acheron/engine/renderer/rr_ar_texturetarget.h>

#include <GL/glew.h>
#include <GL/gl.h>

typedef unsigned ar_texture_id_t;

#define AR_TEXTURE_TYPE_RGB     GL_RGB
#define AR_TEXTURE_TYPE_RGBA    GL_RGBA

typedef struct {
    unsigned id;
    // type of data stored e.g RGB, RGBA, etc.
    int data_type;
    int draw_type;
    int bind_type;
    int index;
    
    image_t image;
} ar_texture_t;

typedef struct {

} ar_texture_t;

ar_texture_t *ar_texture_new(void);
void ar_texture_init(ar_texture_t *ar_texture);
void ar_texture_bind(ar_texture_t *ar_texture);
ar_texture_t *ar_texture_load(ar_texture_t *ar_texture, const char *path, ar_image_type_t type);
ar_texture_t *ar_texture_load_data(ar_texture_t *ar_texture, const char *path, ar_image_type_t type);
void ar_texture_set_data(ar_texture_t *ar_texture, int width, int height, int type_size, unsigned char *data);
void ar_texture_set_parameter(ar_texture_t *ar_texture, int parameter, int value);
void ar_texture_destroy(ar_texture_t *tex);
void ar_textures_cleanup(void);
 
#endif
