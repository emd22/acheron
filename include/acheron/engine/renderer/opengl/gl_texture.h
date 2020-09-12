#ifndef AR_GL_TEXTURE_H
#define AR_GL_TEXTURE_H
#ifdef AR_USE_OPENGL

typedef struct {
    unsigned id;


} ar_intern_texture_t;

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
    unsigned id;
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
#endif