#ifndef F3D_IMAGE_H
#define F3D_IMAGE_H

#include <acheron/engine/image/bmp.h>

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
        image_bmp_t bmp;
    } image;
} image_t;

image_t image_load(const char *path, ar_image_type_t type);
void image_destroy(image_t *image);

#endif
