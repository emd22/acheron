#ifndef F3D_MESH_H
#define F3D_MESH_H

#include <stdbool.h>

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
     
    unsigned primative;

    obj_model_t *obj;
    
    ar_buffer_t vertices;
    ar_buffer_t indices;
    
    unsigned vao, vbo, ibo;
    
    bool initialized;
} mesh_t;

void meshes_init(void);
mesh_t *mesh_new(void);
void mesh_set_data(mesh_t *mesh, ar_buffer_t *vertices, ar_buffer_t *uvs, ar_buffer_t *normals);
mesh_t *mesh_load(mesh_t *mesh, const char *path, int type, int flags);
void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, ar_shader_t *shader);
void meshes_cleanup(void);
void mesh_destroy(mesh_t *mesh);

#endif
