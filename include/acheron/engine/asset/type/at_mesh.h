#ifndef AR_AT_MESH_H
#define AR_AT_MESH_H

#include <stdbool.h>

#include <acheron/engine/model/obj.h>
#include <acheron/engine/types.h>
#include <acheron/engine/scene/sc_camera.h>
#include <acheron/engine/renderer/rr_shader.h>

#define AR_MESH_NO_TANGENTS 0x01
#define AR_MESH_KEEP_DATA   0x02

typedef struct {
    ar_vector3f_t position;
    ar_vector2f_t uv;
    ar_vector3f_t normal;
    ar_vector3f_t tangent;
    ar_vector3f_t bitangent;
} vertex_t;

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
} ar_mesh_t;

void ar_meshes_init(void);
ar_mesh_t *ar_mesh_new(void);
ar_mesh_t *ar_mesh_from_data(ar_mesh_t *mesh, ar_buffer_t *vertices, ar_buffer_t *uvs, ar_buffer_t *normals);
ar_mesh_t *ar_mesh_load(ar_mesh_t *mesh, const char *path, int type, int flags);
void ar_mesh_draw(ar_mesh_t *mesh, ar_mat4_t *matrix, ar_quat_t *rotation, ar_camera_t *camera, ar_shader_t *shader);
void ar_meshes_cleanup(void);
void ar_mesh_destroy(ar_mesh_t *mesh);

#endif
