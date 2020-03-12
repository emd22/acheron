#include <gl/model.h>
#include <gl/model/obj.h>
#include <gl/log.h>

#include <string.h>
#include <stdlib.h>

#include <gl/type/vec.h>

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
    
    return model;
}

void model_destroy(model_t *model) {
    if (model->type == MODEL_NONE)
        return;
        
    if (model->type == MODEL_OBJ) {
        obj_destroy(model->obj);
        model->type = MODEL_NONE;
    }
}
