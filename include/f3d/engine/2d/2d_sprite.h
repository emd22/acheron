#ifndef AR_2D_SPRITE_H
#define AR_2D_SPRITE_H

#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/image/texture.h>
#include <f3d/engine/core/memory/mm_memory.h>

typedef struct {
    ar_vector3f_t position;
    ar_vector3f_t rotation;
    
    texture_t *texture;
} ar_sprite_t;

#endif
