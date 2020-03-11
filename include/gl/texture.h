#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/image.h>

typedef struct {
    unsigned id;
    image_t image;
} texture_t;

texture_t texture_load(const char *path);
void texture_destroy(texture_t *tex);

#endif
