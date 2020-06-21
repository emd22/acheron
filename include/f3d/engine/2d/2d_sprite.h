#ifndef AR_2D_SPRITE_H
#define AR_2D_SPRITE_H

typedef struct {
    ar_vector3f_t position;
    ar_vector3f_t rotation;
    
    texture_t *texture;
} ar_sprite_t;

#endif
