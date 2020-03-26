#ifndef F3D_MODEL_H
#define F3D_MODEL_H

#include <f3d/engine/mesh.h>
#include <f3d/engine/types.h>

typedef struct {
    char name[16];
    mesh_t *mesh;
    mat4_t matrix;
    
    vector3f_t *tangents, *bitangents;
    
    unsigned tangent_id;
    unsigned bitangent_id;
    
    vector3f_t position;
    vector3f_t rotation;
    
    vector3f_t _old_rotation;
    vector3f_t _old_position;
} model_t;

model_t model_load(const char *name, const char *path, int type);
void model_attach(model_t *model, mesh_t *mesh);
void model_update(model_t *model);
void model_draw(model_t *model, camera_t *camera, unsigned shaderid);
void model_destroy(model_t *model);

#endif
