#ifndef ARI_PNG_H
#define ARI_PNG_H

#include <ar_image/ari_def.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    int width, height, pitch;
    int channels;
    ari_pixel_format_t pixel_format;

    uint8_t *data;
} ari_png_t;

ari_error_t ari_png_load(FILE *fp, ari_png_t *png, ari_pixel_format_t pfmt);
ari_error_t ari_png_save(FILE *outfp, ari_png_t *png, ari_pixel_format_t pfmt);
void ari_png_destroy(ari_png_t *png);

#endif
