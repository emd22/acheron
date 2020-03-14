#include <f3d/engine/model.h>
#include <f3d/engine/model/obj.h>
#include <f3d/engine/log.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/type/vec.h>

#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

model_t model_load(const char *path, int type) {
    model_t model;
    model.type = type;
    if (type == MODEL_OBJ) {
        model.obj = malloc(sizeof(model_obj_t));
        model_obj_t obj = obj_load(path);
        memcpy(model.obj, &obj, sizeof(model_obj_t));
        
        model.vertices = &(model.obj->vertices);
        model.uvs = &(model.obj->uvs);
        model.normals = &(model.obj->normals);
    }
    else {
        log_msg(LOG_ERROR, "Cannot load model of unknown type\n", 0);
        model.vertices = NULL;
        model.normals = NULL;
        model.uvs = NULL;
    }
    
    // vertices
    glGenBuffers(1, &model.vertex_id);
    glBindBuffer(GL_ARRAY_BUFFER, model.vertex_id);
    glBufferData(GL_ARRAY_BUFFER, model.vertices->index*sizeof(vector3f_t), model.vertices->data, GL_STATIC_DRAW);

    // colour(uvs)
    glGenBuffers(1, &model.uv_id);
    glBindBuffer(GL_ARRAY_BUFFER, model.uv_id);
    glBufferData(GL_ARRAY_BUFFER, model.uvs->index*sizeof(vector2f_t), model.uvs->data, GL_STATIC_DRAW);
    
    // normals
    glGenBuffers(1, &model.normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, model.normal_id);
    glBufferData(GL_ARRAY_BUFFER, model.normals->index*sizeof(vector3f_t), model.normals->data, GL_STATIC_DRAW);
    
    mat4_set(&model.matrix, MAT4_IDENTITY);
    
    return model;
}

void model_draw(model_t *model, camera_t *camera, unsigned shaderid) {
    mat4_t val = mat4_mul(camera->vp_mat, model->matrix);
    shader_set_mat4(shaderid, "M", &(model->matrix));
    shader_set_mat4(shaderid, "MVP", &val);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, model->vertex_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, model->uv_id);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, NULL);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, model->normal_id);
    glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, NULL);
    
    glDrawArrays(GL_TRIANGLES, 0, model->vertices->size*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void model_destroy(model_t *model) {
    if (model->type == MODEL_NONE)
        return;
        
    if (model->type == MODEL_OBJ) {
        obj_destroy(model->obj);
        model->type = MODEL_NONE;
    }
}
