#ifndef SB_SCENE_H
#define SB_SCENE_H

#include <f3d/engine/scene/scene.h>

typedef struct {
    char name[32];
    
    ar_buffer_t objects;
    ar_buffer_t lights;
} ar_scene_t;

#endif
