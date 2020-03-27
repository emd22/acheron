#ifndef F3D_MODEL_H
#define F3D_MODEL_H

#include <f3d/engine/mesh.h>
#include <f3d/engine/types.h>

typedef struct {
    char name[16];
    int flags;
    
    mesh_t *mesh;
    vector3f_t position, rotation;
    vector3f_t _old_rotation, _old_position;
    mat4_t matrix; 
} model_t;

void model_init(const char *name, model_t *model, int flags);
void model_update(model_t *model);
void model_draw(model_t *model, camera_t *camera, unsigned shaderid);
void model_destroy(model_t *model);

#endif
