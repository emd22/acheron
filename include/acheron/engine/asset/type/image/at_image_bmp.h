#ifndef AR_AT_IMAGE_BMP_H
#define AR_AT_IMAGE_BMP_H

#include <stdint.h>
    
typedef struct {
    uint8_t type[2]; // magic number
    uint32_t file_size;
    uint16_t resv0;
    uint16_t resv1;
    uint32_t offset;
} __attribute__((packed)) ar_bmp_file_header_t;

typedef struct {
    // header size
    uint32_t size;
    int32_t  image_width;
    int32_t  image_height;
    uint16_t colour_planes;
    uint16_t pixel_bit_count;
    uint32_t image_compression;
    uint32_t image_data_size;
    int32_t  pixels_per_meter_x;
    int32_t  pixels_per_meter_y;
    uint32_t colours_used;
    uint32_t important_colours;
} __attribute__((packed)) ar_bmp_info_header_t;

#define BI_RGB       0 /* No compression - straight BGR data */
#define BI_RLE8      1 /* 8-bit run-length compression */
#define BI_RLE4      2 /* 4-bit run-length compression */
#define BI_BITFIELDS 3 /* RGB bitmap with RGB masks */

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char resv0; // alpha(reserved)
} __attribute__((packed)) ar_bmp_rgb_quad_t;
    
typedef struct {
    bmp_info_header_t header;
    // colourmap
    ar_bmp_rgb_quad_t colours[256];
} __attribute__((packed)) ar_bmp_info_t;

typedef struct {
    int flags;
    uint8_t *data;
    ar_bmp_info_t info;
} ar_image_bmp_t;

ar_image_bmp_t ar_bmp_load(const char *filename);
extern int32_t ar_bmp_save(const char *filename, ar_bmp_info_t *info, uint8_t *data);

#endif
