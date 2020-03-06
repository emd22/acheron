#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H

#include <type/vec.h>

typedef struct {
    vec3f_t *vertices;
    vec2f_t *uvs;
    vec3f_t *normals;
} model_obj_t;

#endif
