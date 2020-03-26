#ifndef F3D_MODEL_OBJ_H
#define F3D_MODEL_OBJ_H

#include <f3d/engine/types.h>

typedef struct {
    // vec3f buffer
    buffer_t vertices;
    // vec2f buffer
    buffer_t uvs;
    // vec3f buffer
    buffer_t normals;
} obj_model_t;

obj_model_t obj_load(const char *path);
void obj_destroy(obj_model_t *model);

#endif
