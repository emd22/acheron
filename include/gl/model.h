#ifndef MODEL_H
#define MODEL_H

#include <gl/model/obj.h>

enum {
    MODEL_NONE,
    MODEL_OBJ,
};

typedef struct {
    int type;
    model_obj_t *obj;
    buffer_t *vertices;
    buffer_t *uvs;
    buffer_t *normals;
    
    unsigned vertex_id;
    unsigned uv_id;
    unsigned normal_id; 
} model_t;

model_t model_load(const char *path, int type);
void model_destroy(model_t *model);

#endif
