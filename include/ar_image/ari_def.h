#ifndef ARI_DEF_H
#define ARI_DEF_H

#include <stdint.h>

typedef enum {
    ARI_SUCCESS,
    ARI_FILE_IO_ERROR,
    ARI_FILE_MISSING,
    ARI_ALLOC_ERROR,
    ARI_READ_ERROR,
    ARI_WRITE_ERROR,
    ARI_SIGNATURE_ERROR,
} ari_error_t;

typedef enum {
    ARI_UNSUPPORTED = -2,
    ARI_UNKNOWN     = -1,
    ARI_RGB = 0,
    ARI_BGR,
    ARI_RGBX,
    ARI_BGRX,
    ARI_XBGR,
    ARI_XRGB,
    ARI_GRAY,
    ARI_RGBA,
    ARI_BGRA,
    ARI_ABGR,
    ARI_ARGB,
    ARI_CMYK,
    
    
    ARI_PF_AMT,
} ari_pixel_format_t;

typedef enum {
    ARI_TYPE_AUTO,
    ARI_TYPE_PNG,
    ARI_TYPE_JPEG,
    ARI_TYPE_UNKNOWN,
} ari_image_type_t;

typedef enum {
    ARI_SAMP_444 = 0,
    ARI_SAMP_422,
    ARI_SAMP_420,
    ARI_SAMP_GRAY,
    ARI_SAMP_440,
    ARI_SAMP_411,
} ari_subsamp_t;

#define ARI_TJFLAG_BOTTOMUP      0x0002
#define ARI_TJFLAG_FASTUPSAMPLE  0x0100
#define ARI_TJFLAG_NOREALLOC     0x0400
#define ARI_TJFLAG_FASTDCT       0x0800
#define ARI_TJFLAG_ACCURATEDCT   0x1000
#define ARI_TJFLAG_STOPONWARNING 0x2000
#define ARI_TJFLAG_PROGRESSIVE   0x4000

#endif
