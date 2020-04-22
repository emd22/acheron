#ifndef F3D_MESH_H
#define F3D_MESH_H

#include <f3d/engine/model/obj.h>
#include <f3d/engine/types.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/shader.h>

#define MESH_NO_TANGENTS 0x01
#define MESH_KEEP_DATA   0x02

enum {
    MODEL_NONE,
    MODEL_OBJ,
};

typedef struct {
    int type;
    int index;
    int flags;

    obj_model_t *obj;
    buffer_t *vertices;
    buffer_t *uvs;
    buffer_t *normals;
    
    unsigned vertex_id;
    unsigned uv_id;
    unsigned normal_id;
    
    unsigned vao;
    
    vector3f_t *tangents, *bitangents;
    unsigned tangent_id, bitangent_id;   
} mesh_t;

void meshes_init(void);
mesh_t *mesh_new(void);
mesh_t *mesh_load(mesh_t *mesh, const char *path, int type, int flags);
void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, shader_t *shader);
void meshes_cleanup(void);
void mesh_destroy(mesh_t *mesh);

#endif
