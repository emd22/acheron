#ifndef F3D_TEXTURE_H
#define F3D_TEXTURE_H

#include <f3d/engine/image.h>

#define TEXTURE_KEEP_DATA 0x01

typedef struct {
    unsigned id;
    int flags;
    int index;
    image_t image;
} texture_t;

texture_t *texture_load(const char *path, int type, int flags);
void textures_cleanup_all(void);
void texture_destroy(texture_t *tex);
void textures_cleanup(void);

#endif
