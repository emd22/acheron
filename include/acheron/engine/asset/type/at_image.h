#ifndef AR_AT_IMAGE_H
#define AR_AT_IMAGE_H

#include <acheron/engine/asset/type/image/at_image_bmp.h>

typedef enum {
    AR_IMAGE_NONE,
    AR_IMAGE_BMP,
} ar_image_type_t;

typedef struct {
    ar_image_type_t type;
    int width, height;
    int bpp; // bits per pixel
    uint8_t *data;
    uint16_t flags;
    union {
        ar_image_bmp_t bmp;
    } image;
} ar_image_t;

ar_image_t ar_image_load(const char *path, ar_image_type_t type);
void ar_image_destroy(ar_image_t *image);

#endif
