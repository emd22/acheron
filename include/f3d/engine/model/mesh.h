#ifndef F3D_MESH_H
#define F3D_MESH_H

#include <f3d/engine/model/obj.h>
#include <f3d/engine/types.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/shader.h>

#define MESH_NO_TANGENTS 0x01
#define MESH_KEEP_DATA   0x02

typedef struct {
    vector3f_t position;
    vector2f_t uv;
    vector3f_t normal;
    vector3f_t tangent;
    vector3f_t bitangent;
} __attribute__((packed)) vertex_t;

enum {
    MODEL_NONE,
    MODEL_OBJ,
};

typedef struct {
    int type;
    int index;
    int flags;

    obj_model_t *obj;
    
    buffer_t vertices;
    buffer_t indices;

    /*buffer_t *vertices;
    buffer_t *vertex_indices;
    buffer_t *uvs;
    buffer_t *normals;
    
    unsigned vertex_id;
    unsigned uv_id;
    unsigned normal_id;
    unsigned indices_id;*/
    
    unsigned vao, vbo, ibo;
    
    vector3f_t *tangents, *bitangents;
    unsigned tangent_id, bitangent_id;   
} mesh_t;

void meshes_init(void);
mesh_t *mesh_new(void);
void mesh_set_data(mesh_t *mesh, vector3f_t *vertices, int verts_size, vector2f_t *uvs, int uvs_size, vector3f_t *normals, int norms_size, int flags);
mesh_t *mesh_load(mesh_t *mesh, const char *path, int type, int flags);
void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, shader_t *shader);
void meshes_cleanup(void);
void mesh_destroy(mesh_t *mesh);

#endif
