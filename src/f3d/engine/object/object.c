#include <f3d/engine/object/object.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>

#include <string.h>
#include <stdbool.h>

#define MAX_RENDER_OBJECTS 128

render_object_t render_objects[MAX_RENDER_OBJECTS];
int render_objects_index = 0;

void objects_sort(void);

render_object_t *object_new(const char *name) {
    int index = render_objects_index++;
    if (index > MAX_RENDER_OBJECTS) {
        log_msg(LOG_WARN, "Render objects index > render objects size (%d > %d)\n", render_objects_index, MAX_RENDER_OBJECTS);
        render_objects_index = 0;
        index = 0;
    }
    render_object_t *object = &render_objects[index];
    strcpy(object->name, name);
    object->hash = util_hash_str(name);
    object->mesh = NULL;
    object->position = (vector3f_t){0, 0, 0};
    object->rotation = (vector3f_t){0, 0, 0};
    object->scale = (vector3f_t){1, 1, 1};
    object->material = NULL;
    
    object_update(object);
    
    return object;
}

void render_set_target(int target, void *ptr) {
    if (target == RENDER_TARGET_FRAMEBUFFER) {
        framebuffer_bind((framebuffer_t *)ptr);
    }
    else {
        log_msg(LOG_WARN, "Target is not a valid type\n", 0);
    }
}

render_object_t *render_object_get(const char *name) {
    hash_t hash = util_hash_str(name);
    int i;
    for (i = 0; i < render_objects_index; i++) {
        if (render_objects[i].hash == hash)
            return &render_objects[i];
    }
    return NULL;
}

void object_scale(render_object_t *object) {
    //int i;
    mat4_t *mat = &object->matrix;
    // x
    mat->val[0] *= object->scale.x;
    mat->val[1] *= object->scale.x;
    mat->val[2] *= object->scale.x;
    mat->val[3] *= object->scale.x;
    
    mat->val[4] *= object->scale.y;
    mat->val[5] *= object->scale.y;
    mat->val[6] *= object->scale.y;
    mat->val[7] *= object->scale.y;
    
    mat->val[8] *= object->scale.z;
    mat->val[9] *= object->scale.z;
    mat->val[10] *= object->scale.z;
    mat->val[11] *= object->scale.z;
}

void object_move_v(render_object_t *object, vector3f_t val) {
    object->position = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_rotate_v(render_object_t *object, vector3f_t val) {
    object->rotation = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_move(render_object_t *object, float x, float y, float z) {
    object_move_v(object, (vector3f_t){x, y, z});
}

void object_rotate(render_object_t *object, float x, float y, float z) {
    object_rotate_v(object, (vector3f_t){x, y, z});
}

void object_update(render_object_t *object) {
    // translations
    mat4_translate(&object->matrix, object->position);
    
    // do rotations
    object->matrix = mat4_rotate_x(object->matrix, object->rotation.x);
    object->matrix = mat4_rotate_y(object->matrix, object->rotation.y);
    object->matrix = mat4_rotate_z(object->matrix, object->rotation.z);
    
    object_scale(object);
}

void object_attach(render_object_t *object, int type, void *data) {
    if (type == OBJECT_ATTACH_MESH) {
        object->mesh = (mesh_t *)data;
    }
    else if (type == OBJECT_ATTACH_MATERIAL) {
        object->material = (material_t *)data;
    }
    else {
        log_msg(LOG_ERROR, "Attach type is not valid\n", 0);
    }
}

void objects_sort(void) {
    // material
    render_object_t new_buf[MAX_RENDER_OBJECTS];
    int new_buf_i = 0;
    
    hash_t mat_hashes[MATERIALS_MAX];
    int mat_hashes_i = 0;
    
    render_object_t *object;
    
    bool skip = false;
    int i, j;
    // loop through all objects
    for (i = 0; i < render_objects_index; i++) {
        object = &render_objects[i];
        if (object->material == NULL) {
            log_msg(LOG_INFO, "No material!\n", 0);        
            continue;
        }
        for (j = 0; j < mat_hashes_i; j++) {
            if (object->material->hash != mat_hashes[j]) {
                mat_hashes[mat_hashes_i++] = object->material->hash;
                break;
            }
            else
                skip = true;
        }
        if (skip == true) {
            skip = false;
            continue;
        }
        
        for (j = 0; j < render_objects_index; j++) {
            if (render_objects[j].material->hash == object->material->hash) {
                new_buf[new_buf_i++] = render_objects[j];
            }
        }
    }
    
    for (i = 0; i < new_buf_i; i++) {
        render_objects[i] = new_buf[i];
        log_msg(LOG_INFO, "%s\n", render_objects[i].name);
    }
}

void objects_draw(shader_t *shader, camera_t *camera) {
    int i;
    hash_t mat_hash = 0;
    render_object_t *object;
    for (i = 0; i < render_objects_index; i++) {
        object = &render_objects[i];
        if (mat_hash != object->material->hash) {
            material_update(object->material, shader);
            mat_hash = object->material->hash;
        }
        if (object->flags & RENDER_OBJECT_FLAG_UPDATE) {
            object_update(object);
        }
        if (object->mesh != NULL) {
            mesh_draw(object->mesh, &object->matrix, camera, shader);
        }
    }
}
