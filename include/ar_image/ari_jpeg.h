#ifndef ARI_JPEG_H
#define ARI_JPEG_H

#include <stdio.h>

#include <ar_image/ari_def.h>

typedef struct {
    int width, height, pitch;
    int channels;
    ari_pixel_format_t pixel_format;
    
    uint8_t *data;
} ari_jpeg_t;

ari_error_t ari_jpeg_save(FILE *outfp, ari_jpeg_t *jpeg, ari_pixel_format_t pfmt, int subsamp, int quality, int flags);
void ari_jpeg_destroy(ari_jpeg_t *jpeg);
ari_error_t ari_jpeg_load(FILE *fp, ari_jpeg_t *jpeg, ari_pixel_format_t pfmt);

#endif
