#include <f3d/engine/mesh.h>
#include <f3d/engine/model.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/log.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/types.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GL/gl.h>

model_t model_load(const char *name, const char *path, int type) {
    model_t model;
    mesh_t *mesh = mesh_load(path, type);
    model.mesh = mesh;
    strcpy(model.name, name);
    mat4_set(&model.matrix, MAT4_IDENTITY);
    
    model.position = (vector3f_t){0, 0, 0};
    model.rotation = (vector3f_t){0, 0, 0};
    model._old_position = model.position;
    model._old_rotation = model.rotation;
    
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
    glGenBuffers(1, &model.tangent_id);
    glBindBuffer(GL_ARRAY_BUFFER, model.tangent_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);
    
    // bitangents
    glGenBuffers(1, &model.bitangent_id);
    glBindBuffer(GL_ARRAY_BUFFER, model.bitangent_id);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices->index*sizeof(vector3f_t), mesh->normals->data, GL_STATIC_DRAW);
    
    
    model.tangents = tangents;
    model.bitangents = bitangents;
    return model;
}

void model_attach(model_t *model, mesh_t *mesh) {
    if (model->mesh->type != MODEL_NONE) {
        mesh_destroy(model->mesh);
    }
    model->mesh = mesh;
}

void model_update(model_t *model) {
    mat4_translate(&model->matrix, model->position);
    if (vec3f_cmp(model->position, model->_old_position)) {
        // not equal, lets translate matrix
        mat4_translate(&model->matrix, model->position);
    }
    
    if (model->rotation.x != model->_old_rotation.x)
        model->matrix = mat4_rotate_x(model->matrix, model->rotation.x);
    
    if (model->rotation.y != model->_old_rotation.y)
        model->matrix = mat4_rotate_y(model->matrix, model->rotation.y);
        
    if (model->rotation.z != model->_old_rotation.z)
        model->matrix = mat4_rotate_z(model->matrix, model->rotation.z);
    
    model->_old_position = model->position;
    model->_old_rotation = model->rotation;
}

void model_draw(model_t *model, camera_t *camera, unsigned shaderid) {
    model_update(model);
    mesh_draw(model->mesh, &model->matrix, camera, shaderid, model->tangent_id, model->bitangent_id);
    
}

void model_destroy(model_t *model) {
    mesh_destroy(model->mesh);
    free(model->tangents);
    free(model->bitangents);
}
