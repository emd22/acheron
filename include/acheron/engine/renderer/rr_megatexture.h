#ifndef AR_RR_MEGATEXTURE_H
#define AR_RR_MEGATEXTURE_H

#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/core/memory/mm_buffer.h>

typedef struct {
    int index_x, index_y;
    // size in number of units(megatexture with 512x512 unit size
    // would mean a unit of size 2x1 would be 1024x512).
    int size_x,  size_y;
} ar_megatexture_unit_t;

typedef struct {
    ar_texture_t *texture;
    ar_buffer_t units;
    int width, height;
    int unit_width, unit_height;
} ar_megatexture_t;


#endif