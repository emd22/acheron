#ifndef AR_2D_SPRITE_H
#define AR_2D_SPRITE_H

#include <acheron/engine/math/mt_math.h>
#include <acheron/engine/image/texture.h>
#include <acheron/engine/core/memory/mm_memory.h>

typedef struct {
    ar_vector3f_t position;
    ar_vector3f_t rotation;
    
    texture_t *texture;
} ar_sprite_t;

#endif
