#ifndef F3D_MODEL_OBJ_H
#define F3D_MODEL_OBJ_H

#include <acheron/engine/types.h>

typedef struct {
    int inited;
    // vec3f buffer
    ar_buffer_t vertices;
    // vec2f buffer
    ar_buffer_t uvs;
    // vec3f buffer
    ar_buffer_t normals;
} obj_model_t;

obj_model_t obj_load(const char *path);
void obj_destroy(obj_model_t *model);

#endif
