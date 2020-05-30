#ifndef SB_SCENE_H
#define SB_SCENE_H

typedef struct {
    char name[32];
    
    buffer_t objects;
    buffer_t lights;
} sb_scene_t;

#endif
