#include <f3d/engine/mesh.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/log.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/type/vec.h>

#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MAX_MESHES 512

static mesh_t *meshes[MAX_MESHES];
static int meshes_index = 0;

void calculate_tangents(mesh_t *mesh);

mesh_t *mesh_load(const char *path, int type, int flags) {
    mesh_t *mesh = malloc(sizeof(mesh_t));
    mesh->type = type;
    int index = meshes_index++;
    meshes[index] = mesh;
    mesh->index = index;
    
    if (type == MODEL_OBJ) {
        mesh->obj = malloc(sizeof(obj_model_t));
        obj_model_t obj = obj_load(path);
        memcpy(mesh->obj, &obj, sizeof(obj_model_t));
        
        mesh->vertices = &(mesh->obj->vertices);
        mesh->uvs = &(mesh->obj->uvs);
        mesh->normals = &(mesh->obj->normals);
    }
    else {
        log_msg(LOG_ERROR, "Cannot load mesh of unknown type\n", 0);
        mesh->vertices = NULL;
        mesh->normals = NULL;
        mesh->uvs = NULL;
        return NULL;
    }
    
    // vertices
    glGenBuffers(1, &mesh->vertex_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->vertices->data, GL_STATIC_DRAW);

    // colour(uvs)
    glGenBuffers(1, &mesh->uv_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->uvs->index*sizeof(vector2f_t), mesh->uvs->data, GL_STATIC_DRAW);
    
    // normals
    glGenBuffers(1, &mesh->normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->normals->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);
    
    mesh->flags = flags;
    
    if (!(flags & MESH_NO_TANGENTS)) {
        calculate_tangents(mesh);
    }
    
    return mesh;
}

void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, unsigned shaderid) {
    shader_set_mat4(shaderid, "m", matrix);
    shader_set_mat4(shaderid, "v", &camera->mat_view);
    shader_set_mat4(shaderid, "p", &camera->mat_projection);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_id);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_id);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, NULL);
    
    if (!(mesh->flags & MESH_NO_TANGENTS)) {
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->tangent_id);
        glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, NULL);
        
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->bitangent_id);
        glVertexAttribPointer(4, 3, GL_FLOAT, 0, 0, NULL);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertices->size*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    
    if (!(mesh->flags & MESH_NO_TANGENTS)) {
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
    }
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
        mesh_destroy(meshes[i]);
        free(meshes[i]);
    }
}

void mesh_destroy(mesh_t *mesh) {
    if (mesh->type == MODEL_NONE)
        return;
    log_msg(LOG_INFO, "Deleting mesh(idx: %d)\n", mesh->index);
        
        
    if (mesh->type == MODEL_OBJ) {
        obj_destroy(mesh->obj);
    }
    if (!(mesh->flags & MESH_NO_TANGENTS)) {
        free(mesh->tangents);
        free(mesh->bitangents);
    }
    mesh->type = MODEL_NONE;
}
