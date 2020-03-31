#ifndef F3D_MODEL_F3B_H
#define F3D_MODEL_F3B_H

#include <stdint.h>

typedef struct {
    uint8_t ident[2];      //  2
    uint16_t revision;     //  4
    uint32_t size;         //  8
    uint32_t vert_start;   // 12
    uint32_t uv_start;     // 16
    uint32_t normal_start; // 20
    uint8_t resv0[26];
} f3b_header_t; // 48 byte header

#endif
