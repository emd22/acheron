#include <f3d/engine/mesh.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/log.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/type/vec.h>

#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

mesh_t *mesh_load(const char *path, int type) {
    mesh_t *mesh = malloc(sizeof(mesh_t));
    mesh->type = type;
    if (type == MODEL_OBJ) {
        mesh->obj = malloc(sizeof(obj_model_t));
        obj_model_t obj = obj_load(path);
        memcpy(mesh->obj, &obj, sizeof(obj_model_t));
        
        mesh->vertices = &(mesh->obj->vertices);
        mesh->uvs = &(mesh->obj->uvs);
        mesh->normals = &(mesh->obj->normals);
    }
    else {
        log_msg(LOG_ERROR, "Cannot load model of unknown type\n", 0);
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
    
    return mesh;
}

void mesh_draw(mesh_t *mesh, mat4_t *matrix, camera_t *camera, unsigned shaderid, unsigned tangent_id, unsigned bitangent_id) {
    shader_set_mat4(shaderid, "M", matrix);
    shader_set_mat4(shaderid, "V", &camera->mat_view);
    shader_set_mat4(shaderid, "P", &camera->mat_projection);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_id);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_id);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangent_id);
    glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangent_id);
    glVertexAttribPointer(4, 3, GL_FLOAT, 0, 0, NULL);
    
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertices->size*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

void mesh_destroy(mesh_t *mesh) {
    if (mesh == NULL)
        return;
        
    if (mesh->type == MODEL_OBJ) {
        obj_destroy(mesh->obj);
        mesh->type = MODEL_NONE;
    }
    free(mesh);
}
