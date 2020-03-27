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

void model_init(const char *name, model_t *model, int flags) {
    model->position = (vector3f_t){0, 0, 0};
    model->rotation = (vector3f_t){0, 0, 0};
    model->_old_position = model->position;
    model->_old_rotation = model->rotation;
    
    mat4_set(&model->matrix, MAT4_IDENTITY);
    
    model->flags = flags;
        
    strcpy(model->name, name);
}

void model_update(model_t *model) {
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
    mesh_draw(model->mesh, &model->matrix, camera, shaderid);
}

void model_destroy(model_t *model) {
    mesh_destroy(model->mesh);
}
