#include <f3d/engine/scene/object.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>
#include <f3d/engine/types.h>
#include <f3d/engine/limits.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void objects_sort(object_buffer_t *objects);

object_t object_new(const char *name) {
    object_t object;
    
    strcpy(object.name, name);
    object.hash = util_hash_str(name);
    object.mesh = NULL;
    object.position = (vector3f_t){0, 0, 0};
    object.rotation = (vector3f_t){0, 0, 0};
    object.scale = (vector3f_t){1, 1, 1};
    object.material = NULL;
    
    object.physics = physics_object_new(PHYSICS_COLLIDER_AABB);
    
    object_update(&object);
    
    return object;
}

void render_set_target(int target, void *ptr) {
    if (target == RENDER_TARGET_FRAMEBUFFER) {
        framebuffer_bind((framebuffer_t *)ptr);
    }
    else {
        ar_log(AR_LOG_WARN, "Target is not a valid type\n", 0);
    }
}

object_buffer_t object_buffer_new(buffer_type_t buffer_type, int buffer_start_size) {
    object_buffer_t buffer;
    buffer_init(&buffer.buffer, buffer_type, sizeof(object_t), buffer_start_size);
    buffer.sorted = false;
    return buffer;
}

void scale_object(object_t *object) {
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

void object_move_v(object_t *object, vector3f_t val) {
    object->position = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_rotate_v(object_t *object, vector3f_t val) {
    object->rotation = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}
void object_scale_v(object_t *object, vector3f_t val) {
    object->scale = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_move(object_t *object, float x, float y, float z) {
    object_move_v(object, (vector3f_t){x, y, z});
}

void object_rotate(object_t *object, float x, float y, float z) {
    object_rotate_v(object, (vector3f_t){x, y, z});
}

void object_scale(object_t *object, float x, float y, float z) {
    object_scale_v(object, (vector3f_t){x, y, z});
}

void object_update(object_t *object) {
    // translations
    mat4_translate(&object->matrix, object->position);
    
    // do rotations
    object->matrix = mat4_rotate_x(object->matrix, object->rotation.x);
    object->matrix = mat4_rotate_y(object->matrix, object->rotation.y);
    object->matrix = mat4_rotate_z(object->matrix, object->rotation.z);
    
    scale_object(object);
    object->physics.collider.position = object->position;
    object->physics.collider.scale = object->scale;
}

void object_attach(object_t *object, int type, void *data) {
    if (type == OBJECT_ATTACH_MESH) {
        object->mesh = (mesh_t *)data;
        physics_collider_stretch_to_vertices(&object->physics.collider, &object->mesh->vertices);
    }
    else if (type == OBJECT_ATTACH_MATERIAL) {
        object->material = (material_t *)data;
    }
    else {
        ar_log(AR_LOG_ERROR, "Attach type is not valid\n", 0);
    }
}

hash_t get_material_hash(object_t *object) {
    if (object->material == NULL)
        return 0;
    return object->material->hash;
}

int compare_materials(const void *v1, const void *v2) {
    object_t *obj1 = (object_t *)v1;
    object_t *obj2 = (object_t *)v2;

    return get_material_hash(obj1)-get_material_hash(obj2);
}

void objects_sort(ar_object_buffer_t *objects) {
    object_t *obj_buffer = (object_t *)objects->buffer.data;
    qsort(obj_buffer, objects->buffer.index, sizeof(object_t), &compare_materials);
}

void object_draw(object_t *object, shader_t *shader, camera_t *camera) {
    if (object->flags & RENDER_OBJECT_FLAG_UPDATE) {
        object_update(object);
    }
    if (object->mesh != NULL) {
        mesh_draw(object->mesh, &object->matrix, camera, shader);
    }
}

void objects_draw(object_buffer_t *objects, shader_t *shader, camera_t *camera, bool render_materials) {
    if (objects->sorted == false)
        objects_sort(objects);
    
    unsigned i;
    hash_t mat_hash = 1;
    object_t *object;
    for (i = 0; i < objects->buffer.index; i++) {
        object = &(((object_t *)objects->buffer.data)[i]);
        if (render_materials == false) {
            material_update(NULL, shader);
        }
        else {
            if (mat_hash != get_material_hash(object)) {
                material_update(object->material, shader);
                mat_hash = get_material_hash(object);
            }        
        }
        object_draw(object, shader, camera);
    }
}
