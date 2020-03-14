#ifndef IMAGE_H
#define IMAGE_H

#include <f3d/engine/bmp.h>

enum {
    IMAGE_NONE,
    IMAGE_BMP,
};

typedef struct {
    int type;
    int width, height;
    uint8_t *data;
    uint16_t flags;
    union {
        image_bmp_t bmp;
    } image;
} image_t;

image_t image_load(const char *path, int type);
void image_destroy(image_t *image);

#endif
