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
    //model->_old_position = model->position;
    //model->_old_rotation = model->rotation;
    
    // set matrix to default, with no transformations
    // (1, 0, 0, 0)
    // (0, 1, 0, 0)
    // (0, 0, 1, 0)
    // (0, 0, 0, 1)
    mat4_set(&model->matrix, MAT4_IDENTITY);
    
    model->flags = flags;
    
    // model has name string for easier debugging
    strcpy(model->name, name);
}

void model_update(model_t *model) {
    // TODO: detect when to do rotations automatically

    // translate matrix first
    mat4_translate(&model->matrix, model->position);
    
    // do rotations
    model->matrix = mat4_rotate_x(model->matrix, model->rotation.x);
    model->matrix = mat4_rotate_y(model->matrix, model->rotation.y);
    model->matrix = mat4_rotate_z(model->matrix, model->rotation.z);
    
    //model->_old_position = model->position;
    //model->_old_rotation = model->rotation;
}

void model_draw(model_t *model, camera_t *camera, shader_t *shader) {
    mesh_draw(model->mesh, &model->matrix, camera, shader);
}

void model_destroy(model_t *model) {
    mesh_destroy(model->mesh);
}
