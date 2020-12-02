#ifndef AR_RR_TEXTURE_H
#define AR_RR_TEXTURE_H

#include <ar_image/ar_image.h>

typedef unsigned ar_texture_id_t;

#define AR_TEXTURE_MIPMAP   0x01
#define AR_TEXTURE_EXISTS   0x02

typedef enum {
    AR_TEXTURE_RGB,
    AR_TEXTURE_RGBA,
    AR_TEXTURE_BGR,
    AR_TEXTURE_BGRA,
} ar_texture_data_type_t;

typedef enum {
    AR_TEXTURE_BYTE,
    AR_TEXTURE_SHORT,
    AR_TEXTURE_INT,
} ar_texture_data_width_t;

typedef enum {
    AR_TEXTURE_2D,
    AR_TEXTURE_CUBEMAP,
} ar_texture_bind_type_t;

typedef enum {
    AR_TEXTURE_MIN_FILTER,
    AR_TEXTURE_MAG_FILTER,
    AR_TEXTURE_WRAP_S,
    AR_TEXTURE_WRAP_T,
} ar_texture_parameter_t;

typedef enum {
    AR_TEXTURE_LINEAR,
    AR_TEXTURE_LINEAR_MIPMAP,
    AR_TEXTURE_REPEAT,
} ar_texture_parameter_value_t;

typedef enum {
    AR_TEXTURE_UNIT_NONE = -1,
    AR_TEXTURE_UNIT0 = 0,
    AR_TEXTURE_UNIT1,
    AR_TEXTURE_UNIT2,
    AR_TEXTURE_UNIT3,
    AR_TEXTURE_UNIT4,
    AR_TEXTURE_UNIT5,
    AR_TEXTURE_UNIT6,
    AR_TEXTURE_UNIT7,
} ar_texture_unit_t;

typedef struct ar_texture_s {
    ar_texture_id_t id;
    ari_image_t image;

    int width, height;
    unsigned lod;

    void (*update)(struct ar_texture_s *);
    void (*bind)(struct ar_texture_s *);
    void (*bind_to)(struct ar_texture_s *, ar_texture_unit_t);

    ar_texture_unit_t texture_unit;

    ar_texture_data_width_t data_width;
    ar_texture_bind_type_t bind_type;
    ar_texture_data_type_t draw_type;
    ar_texture_data_type_t data_type;

    int flags;
} ar_texture_t;

ar_texture_t *ar_texture_new(int flags);
void ar_texture_set_parameter(ar_texture_t *texture, ar_texture_parameter_t param, ar_texture_parameter_value_t value);
ari_image_t *ar_texture_get_image(ar_texture_t *texture);
void ar_texture_update(ar_texture_t *texture);
void ar_texture_set_data(ar_texture_t *texture, int width, int height, ar_texture_data_type_t data_type, ar_texture_data_width_t data_width, void *data);
void ar_texture_bind_to(ar_texture_t *texture, ar_texture_unit_t texture_unit);
void ar_texture_bind(ar_texture_t *texture);
unsigned ar_gl_texture_bind_type(ar_texture_bind_type_t type);
void ar_texture_destroy(ar_texture_t *texture);
void ar_texture_buffer_destroy(void);

#endif
