#ifndef F3D_MESH_H
#define F3D_MESH_H

#include <f3d/engine/model/obj.h>
#include <f3d/engine/types.h>
#include <f3d/engine/camera.h>

enum {
    MODEL_NONE,
    MODEL_OBJ,
};


typedef struct {
    int type;
    obj_model_t *obj;
    buffer_t *vertices;
    buffer_t *uvs;
    buffer_t *normals;
    
    unsigned vertex_id;
    unsigned uv_id;
    unsigned normal_id;
} mesh_t;

mesh_t *mesh_load(const char *path, int type);
void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, unsigned shaderid, unsigned tangent_id, unsigned bitangent_id);
void mesh_destroy(mesh_t *mesh);

#endif
