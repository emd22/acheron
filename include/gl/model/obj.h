#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H

#include <gl/type/buffer.h>

typedef struct {
    // vec3f buffer
    buffer_t vertices;
    // vec2f buffer
    buffer_t uvs;
    // vec3f buffer
    buffer_t normals;
} model_obj_t;

model_obj_t obj_load(const char *path);

#endif
