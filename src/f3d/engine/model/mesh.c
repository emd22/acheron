#include <f3d/engine/model/mesh.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/type/vec.h>
#include <f3d/engine/limits.h>

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <GL/glew.h>
#include <GL/gl.h>

static mesh_t *meshes;
static int meshes_index = 0;

void calculate_tangents(mesh_t *mesh);

void meshes_init(void) {
    meshes = malloc(sizeof(mesh_t)*MAX_MESHES);
}

void generate_indices(mesh_t *mesh) {
    unsigned i;
    buffer_init(&mesh->indices, sizeof(unsigned), 2048);
    for (i = 0; i < mesh->vertices.index; i++) {
        
        buffer_push(&mesh->indices, &i);
    }
    log_msg(LOG_INFO, "Generating indices...\n", 0);
}

void gen_packed_verts(mesh_t *mesh, obj_model_t *obj) {
    unsigned i;
    buffer_init(&mesh->vertices, sizeof(vertex_t), 4096);
    vertex_t vertex;
    for (i = 0; i < obj->vertices.index; i++) {
        vertex.position = ((vector3f_t *)obj->vertices.data)[i];
        vertex.uv = ((vector2f_t *)obj->uvs.data)[i];
        vertex.normal = ((vector3f_t *)obj->normals.data)[i];
        buffer_push(&mesh->vertices, &vertex);
    }
}

mesh_t *mesh_new(void) {
    int index = meshes_index++;
    mesh_t *mesh = &meshes[index];
    mesh->type = MODEL_NONE;
    mesh->index = index;
    /*mesh->normals = NULL;
    mesh->uvs = NULL;
    mesh->vertices = NULL;*/
    return mesh;    
}

void mesh_init(mesh_t *mesh, int flags) {
    // generate mesh buffers
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->ibo);
    glGenBuffers(1, &mesh->vbo);
    
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    // TODO: implement methods with dynamic drawing instead of just static
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.index*sizeof(vertex_t), mesh->vertices.data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.index*sizeof(unsigned), mesh->indices.data, GL_STATIC_DRAW);
    
    mesh->flags = flags;
    
    if (!(flags & MESH_NO_TANGENTS)) {
        calculate_tangents(mesh);
    }
    
    int stride = sizeof(vertex_t);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, stride, NULL);
    // uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, uv));
    // normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, normal));
    // tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, tangent));
    // bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, 0, stride, (void *)offsetof(vertex_t, bitangent));
    
}

/*void mesh_set_data(
    mesh_t *mesh, 
    vector3f_t *vertices, int verts_size,
    vector2f_t *uvs,      int uvs_size,
    vector3f_t *normals,  int norms_size,
    int flags)
{
    if (vertices != NULL) {
        unsigned data_size = verts_size*sizeof(vector3f_t);
        mesh->vertices = malloc(sizeof(buffer_t));
        
        mesh->vertices->data = malloc(data_size);
        memcpy(mesh->vertices->data, vertices, data_size);
        mesh->vertices->index = verts_size;
        mesh->vertices->obj_sz = sizeof(vector3f_t);
        mesh->vertices->size = verts_size;
    }
    if (uvs != NULL) {
        unsigned data_size = uvs_size*sizeof(vector2f_t);
        
        mesh->uvs->data = malloc(data_size);
        memcpy(mesh->uvs->data, uvs, data_size);
        mesh->uvs->index = uvs_size;
        mesh->uvs->obj_sz = sizeof(vector2f_t);
        mesh->uvs->size = uvs_size;
    }
    if (normals != NULL) {
        unsigned data_size = norms_size*sizeof(vector3f_t);
        
        mesh->normals->data = malloc(data_size);
        memcpy(mesh->normals->data, normals, data_size);
        mesh->normals->index = norms_size;
        mesh->normals->obj_sz = sizeof(vector3f_t);
        mesh->normals->size = norms_size;
    }
    mesh_init(mesh, flags);
}*/

mesh_t *mesh_load(mesh_t *mesh, const char *path, int type, int flags) {
    if (mesh == NULL)
        mesh = mesh_new();
        
    if (type == MODEL_OBJ) {
        mesh->obj = malloc(sizeof(obj_model_t));
        obj_model_t obj = obj_load(path);
        memcpy(mesh->obj, &obj, sizeof(obj_model_t));
        
        gen_packed_verts(mesh, &obj);
        generate_indices(mesh);
    }
    else {
        log_msg(LOG_ERROR, "Cannot load mesh of unknown type\n", 0);
        /*mesh->vertices = NULL;
        mesh->normals = NULL;
        mesh->uvs = NULL;
        mesh->vertex_indices = NULL;*/
        return NULL;
    }
    
    mesh_init(mesh, flags);
    
    return mesh;
}

void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, shader_t *shader) {
    glDisable(GL_CULL_FACE);
    if (mesh == NULL/* || mesh->vertices == NULL*/)
        return;
        
    if (camera != NULL && matrix != NULL && shader != NULL) {
        shader_set_mat4(shader, "m", matrix);
        shader_set_mat4(shader, "v", &camera->mat_view);
        shader_set_mat4(shader, "p", &camera->mat_projection);    
    }
    
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indices.index, GL_UNSIGNED_INT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, mesh->vertices->size*3);
    //glBindVertexArray(0);
}

void calculate_tangents(mesh_t *mesh) {
    vector3f_t v0, v1, v2;
    vector2f_t uv0, uv1, uv2;

    vertex_t *vertices = (vertex_t *)mesh->vertices.data;
    
    vector3f_t delta_pos0, delta_pos1;
    vector2f_t delta_uv0, delta_uv1;
    vector3f_t tangent, bitangent;
    float r;
        
    vector3f_t tmp;
    
    unsigned i;
    unsigned bufidx = 0;
    for (i = 0; i < mesh->vertices.index; i += 3) {
        v0 = vertices[i].position;
        v1 = vertices[i+1].position;
        v2 = vertices[i+2].position;
        
        uv0 = vertices[i].uv;
        uv1 = vertices[i+1].uv;
        uv2 = vertices[i+2].uv;
        
        vec3f_sub(&delta_pos0, v1, v0);
        vec3f_sub(&delta_pos1, v2, v0);
        vec2f_sub(&delta_uv0, uv1, uv0);
        vec2f_sub(&delta_uv1, uv2, uv0);
        
        r = 1.0f/(delta_uv0.x*delta_uv1.y-delta_uv0.y*delta_uv1.x);
        
        vec3f_sub(&tmp, vec3f_mul_v(delta_pos0, delta_uv1.y), vec3f_mul_v(delta_pos1, delta_uv0.y));
        tangent = vec3f_mul_v(tmp, r);
        
        vec3f_sub(&tmp, vec3f_mul_v(delta_pos1, delta_uv0.x), vec3f_mul_v(delta_pos0, delta_uv1.x));
        bitangent = vec3f_mul_v(tmp, r);
        
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
        vertices[bufidx].bitangent = bitangent;
        vertices[bufidx++].tangent = tangent;
    }
}

void meshes_cleanup(void) {
    int i;
    for (i = 0; i < meshes_index; i++) {
        mesh_destroy(&meshes[i]);
    }
    free(meshes);
}

void mesh_destroy(mesh_t *mesh) {
    if (mesh == NULL || mesh->type == MODEL_NONE)
        return;
    //log_msg(LOG_INFO, "Deleting mesh(idx: %d)\n", mesh->index);
        
    if (mesh->type == MODEL_OBJ) {
        obj_destroy(mesh->obj);
    }
    mesh->type = MODEL_NONE;
}
