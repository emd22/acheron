#include <f3d/engine/model/mesh.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/type/vec.h>
#include <f3d/engine/limits.h>

#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

static mesh_t *meshes;
static int meshes_index = 0;

void calculate_tangents(mesh_t *mesh);

void meshes_init(void) {
    meshes = malloc(sizeof(mesh_t)*MAX_MESHES);
}

mesh_t *mesh_new(void) {
    int index = meshes_index++;
    mesh_t *mesh = &meshes[index];
    mesh->type = MODEL_NONE;
    mesh->index = index;
    mesh->normals = NULL;
    mesh->uvs = NULL;
    mesh->vertices = NULL;
    return mesh;    
}


void mesh_init(mesh_t *mesh, int flags) {
    if (mesh->vertices) {
        // vertices
        glGenBuffers(1, &mesh->vertex_id);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->vertices->data, GL_STATIC_DRAW);
    }
    if (mesh->uvs) {
        // uvs
        glGenBuffers(1, &mesh->uv_id);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_id);
        glBufferData(GL_ARRAY_BUFFER, mesh->uvs->index*sizeof(vector2f_t), mesh->uvs->data, GL_STATIC_DRAW);    
    }
    if (mesh->normals) {
        // normals
        glGenBuffers(1, &mesh->normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_id);
        glBufferData(GL_ARRAY_BUFFER, mesh->normals->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);    
    }
    glGenBuffers(1, &mesh->indices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
    log_msg(LOG_WARN, "%d\n", mesh->vertex_indices->index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->vertex_indices->index*sizeof(unsigned), mesh->vertex_indices->data, GL_STATIC_DRAW);
    glGenVertexArrays(1, &mesh->vao);
    
    mesh->flags = flags;
    
    if (!(flags & MESH_NO_TANGENTS)) {
        calculate_tangents(mesh);
    }
    if (!(flags & MESH_KEEP_DATA)) {
        // we already have our data in VRAM and tangents are calculated, no need
        // to keep this in system memory
        buffer_destroy(mesh->vertices);
        buffer_destroy(mesh->uvs);
        buffer_destroy(mesh->normals);
    }
    
    glBindVertexArray(mesh->vao);
    
    if (mesh->vertices) {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);
    }
    if (mesh->uvs) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_id);
        glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, NULL);
    }
    
    if (mesh->normals) {
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_id);
        glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, NULL);    
    }
    
    if (!(mesh->flags & MESH_NO_TANGENTS)) {
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->tangent_id);
        glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, NULL);
        
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->bitangent_id);
        glVertexAttribPointer(4, 3, GL_FLOAT, 0, 0, NULL);
    }
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
}

void mesh_set_data(
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
}

mesh_t *mesh_load(mesh_t *mesh, const char *path, int type, int flags) {
    if (mesh == NULL)
        mesh = mesh_new();
        
    if (type == MODEL_OBJ) {
        mesh->obj = malloc(sizeof(obj_model_t));
        obj_model_t obj = obj_load(path);
        memcpy(mesh->obj, &obj, sizeof(obj_model_t));
        
        mesh->vertices = &(mesh->obj->vertices);
        mesh->vertex_indices = &(mesh->obj->vertex_indices);
        
        mesh->uvs = &(mesh->obj->uvs);
        mesh->normals = &(mesh->obj->normals);
    }
    else {
        log_msg(LOG_ERROR, "Cannot load mesh of unknown type\n", 0);
        mesh->vertices = NULL;
        mesh->normals = NULL;
        mesh->uvs = NULL;
        mesh->vertex_indices = NULL;
        return NULL;
    }
    
    mesh_init(mesh, flags);
    
    return mesh;
}

void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, shader_t *shader) {
    if (mesh == NULL || mesh->vertices == NULL)
        return;
        
    if (camera != NULL && matrix != NULL && shader != NULL) {
        shader_set_mat4(shader, "m", matrix);
        shader_set_mat4(shader, "v", &camera->mat_view);
        shader_set_mat4(shader, "p", &camera->mat_projection);    
    }
    
    //glBindVertexArray(mesh->vao);
    //glDrawElements(GL_TRIANGLES, mesh->vertex_indices->index, GL_UNSIGNED_INT, NULL);
    //glDrawArrays(GL_TRIANGLES, 0, mesh->vertices->size*3);
}

void calculate_tangents(mesh_t *mesh) {
    vector3f_t v0, v1, v2;
    vector2f_t uv0, uv1, uv2;
    vector3f_t *vertices = (vector3f_t *)mesh->vertices->data;
    vector2f_t *uvs = (vector2f_t *)mesh->uvs->data;
    vector3f_t delta_pos0, delta_pos1;
    vector2f_t delta_uv0, delta_uv1;
    vector3f_t tangent, bitangent;
    float r;
        
    vector3f_t *tangents, *bitangents;
    vector3f_t tmp;
    
    tangents = malloc(sizeof(vector3f_t)*3*mesh->vertices->index);
    bitangents = malloc(sizeof(vector3f_t)*3*mesh->vertices->index);
    
    unsigned i;
    unsigned bufidx = 0;
    for (i = 0; i < mesh->vertices->index; i += 3) {
        v0 = vertices[i];
        v1 = vertices[i+1];
        v2 = vertices[i+2];
        
        uv0 = uvs[i];
        uv1 = uvs[i+1];
        uv2 = uvs[i+2];
        
        vec3f_sub(&delta_pos0, v1, v0);
        vec3f_sub(&delta_pos1, v2, v0);
        vec2f_sub(&delta_uv0, uv1, uv0);
        vec2f_sub(&delta_uv1, uv2, uv0);
        
        r = 1.0f/(delta_uv0.x*delta_uv1.y-delta_uv0.y*delta_uv1.x);
        
        vec3f_sub(&tmp, vec3f_mul_v(delta_pos0, delta_uv1.y), vec3f_mul_v(delta_pos1, delta_uv0.y));
        tangent = vec3f_mul_v(tmp, r);
        
        vec3f_sub(&tmp, vec3f_mul_v(delta_pos1, delta_uv0.x), vec3f_mul_v(delta_pos0, delta_uv1.x));
        bitangent = vec3f_mul_v(tmp, r);
        
        bitangents[bufidx] = bitangent;
        tangents[bufidx++] = tangent;
        bitangents[bufidx] = bitangent;
        tangents[bufidx++] = tangent;
        bitangents[bufidx] = bitangent;
        tangents[bufidx++] = tangent;
    }
    
    // tangents
    glGenBuffers(1, &mesh->tangent_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->tangent_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);
    
    // bitangents
    glGenBuffers(1, &mesh->bitangent_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->bitangent_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);
    
    mesh->tangents = tangents;
    mesh->bitangents = bitangents;
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
    if (!(mesh->flags & MESH_NO_TANGENTS)) {
        free(mesh->tangents);
        free(mesh->bitangents);
    }
    mesh->type = MODEL_NONE;
}
