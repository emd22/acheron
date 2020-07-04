#include <acheron/engine/asset/type/at_image.h>
#include <acheron/engine/image/bmp.h>
#include <stdlib.h>

static void load_bmp(ar_image_t *image, const char *path) {
    ar_image_bmp_t *bmp = &(image->image.bmp);
    (*bmp) = ar_bmp_load(path);
    if (bmp->data == NULL) {
        image->type = AR_IMAGE_NONE;
        return;
    }
    image->type   = AR_IMAGE_BMP;
    image->data   = bmp->data;
    image->width  = bmp->info.header.image_width;
    image->height = bmp->info.header.image_height;
    image->bpp    = bmp->info.header.pixel_bit_count;
}

ar_image_t ar_image_load(const char *path, ar_image_type_t type) {
    ar_image_t image;
    image.flags = 0;
    image.type = AR_IMAGE_NONE;
    image.bpp = 0;
    switch (type) {
        case AR_IMAGE_BMP:
            load_bmp(&image, path);
            break;
        default:
            break;
    };
    return image;
}

void ar_image_destroy(ar_image_t *image) {
    if (image->data != NULL || image->type == AR_IMAGE_NONE)
        free(image->data);
        
    image->data = NULL;
    image->type = AR_IMAGE_NONE;
}
