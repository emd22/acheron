#ifndef AR_RR_TEXTURE_H
#define AR_RR_TEXTURE_H

#include <ar_image/ar_image.h>

typedef long ar_texture_id_t;

typedef enum {
    AR_TEXTURE_RGB,
    AR_TEXTURE_RGBA,
    AR_TEXTURE_BGR,
    AR_TEXTURE_BGRA,
} ar_texture_data_type_t;

typedef enum {
    AR_TEXTURE_BYTE,
    AR_TEXTURE_SHORT,
    AR_TEXTURE_LONG,
} ar_texture_data_width_t;

typedef enum {
    AR_TEXTURE_2D,
} ar_texture_bind_type_t;

typedef struct {
    ar_texture_id_t id;
    ari_image_t image;

    ar_texture_data_width_t data_width;
    ar_texture_bind_type_t bind_type;
    ar_texture_data_type_t draw_type;
    ar_texture_data_type_t data_type;
} ar_texture_t;


ar_texture_t *ar_texture_new(void);
void ar_texture_init(ar_texture_t *ar_texture);
void ar_texture_bind(ar_texture_t *ar_texture);
ar_texture_t *ar_texture_load(ar_texture_t *ar_texture, const char *path, ari_image_type_t type);
ar_texture_t *ar_texture_load_data(ar_texture_t *ar_texture, const char *path, ari_image_type_t type);
void ar_texture_set_data(ar_texture_t *ar_texture, int width, int height, int type_size, unsigned char *data);
void ar_texture_set_parameter(ar_texture_t *ar_texture, int parameter, int value);
void ar_texture_destroy(ar_texture_t *tex);
void ar_textures_cleanup(void);


ar_texture_t *ar_texture_new(void);


void ar_texture_destroy(ar_texture_t *texture);




/*** OLD ***/
/*
ar_texture_t *ar_texture_new(void);
void ar_texture_init(ar_texture_t *ar_texture);
void ar_texture_bind(ar_texture_t *ar_texture);
ar_texture_t *ar_texture_load(ar_texture_t *ar_texture, const char *path, ar_image_type_t type);
ar_texture_t *ar_texture_load_data(ar_texture_t *ar_texture, const char *path, ar_image_type_t type);
void ar_texture_set_data(ar_texture_t *ar_texture, int width, int height, int type_size, unsigned char *data);
void ar_texture_set_parameter(ar_texture_t *ar_texture, int parameter, int value);
void ar_texture_destroy(ar_texture_t *tex);
void ar_textures_cleanup(void);
 
*/ 
#endif
