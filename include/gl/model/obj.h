#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H

#include <gl/type/vec.h>

typedef struct {
    vector3f_t *vertices;
    vector2f_t *uvs;
    vector3f_t *normals;
} model_obj_t;

#endif
