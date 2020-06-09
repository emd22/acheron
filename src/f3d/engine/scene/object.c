#include <f3d/engine/scene/object.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>
#include <f3d/engine/types.h>
#include <f3d/engine/limits.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static ar_buffer_t object_buffer;
static bool objects_sorted = false;

void ar_objects_sort(ar_object_t *objects, int objects_size);

ar_object_t *ar_object_new(const char *name) {
    if (object_buffer.initialized == false) {
        ar_buffer_init(&object_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_object_t), DEFAULT_OBJECTS);
    }
    ar_object_t *object = ar_buffer_new_item(&object_buffer);
    
    strcpy(object->name, name);
    object->hash = util_hash_str(name);
    object->mesh = NULL;
    object->position = (vector3f_t){0, 0, 0};
    object->rotation = (vector3f_t){0, 0, 0};
    object->scale = (vector3f_t){1, 1, 1};
    object->material = NULL;
    
    object->physics = physics_object_new(PHYSICS_COLLIDER_AABB);
    
    ar_object_update(object);
    
    return object;
}

void render_set_target(int target, void *ptr) {
    if (target == AR_RENDER_TARGET_FRAMEBUFFER) {
        ar_framebuffer_bind((ar_framebuffer_t *)ptr);
    }
    else {
        ar_log(AR_LOG_WARN, "Target is not a valid type\n", 0);
    }
}

void scale_object(ar_object_t *object) {
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

void object_move_v(ar_object_t *object, vector3f_t val) {
    object->position = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}

void object_rotate_v(ar_object_t *object, vector3f_t val) {
    object->rotation = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}
void object_scale_v(ar_object_t *object, vector3f_t val) {
    object->scale = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}

void object_move(ar_object_t *object, float x, float y, float z) {
    object_move_v(object, (vector3f_t){x, y, z});
}

void object_rotate(ar_object_t *object, float x, float y, float z) {
    object_rotate_v(object, (vector3f_t){x, y, z});
}

void object_scale(ar_object_t *object, float x, float y, float z) {
    object_scale_v(object, (vector3f_t){x, y, z});
}

void ar_object_update(ar_object_t *object) {
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

void ar_object_attach(ar_object_t *object, int type, void *data) {
    if (type == AR_OBJECT_ATTACH_MESH) {
        object->mesh = (mesh_t *)data;
        physics_collider_stretch_to_vertices(&object->physics.collider, &object->mesh->vertices);
    }
    else if (type == AR_OBJECT_ATTACH_MATERIAL) {
        object->material = (material_t *)data;
    }
    else {
        ar_log(AR_LOG_ERROR, "Attach type is not valid\n", 0);
    }
}

static hash_t get_material_hash(ar_object_t *object) {
    if (object->material == NULL)
        return 0;
    return object->material->hash;
}

static int compare_materials(const void *v1, const void *v2) {
    ar_object_t *obj1 = (ar_object_t *)v1;
    ar_object_t *obj2 = (ar_object_t *)v2;

    return get_material_hash(obj1)-get_material_hash(obj2);
}

void ar_objects_sort(ar_object_t *objects, int objects_size) {
    ar_object_t *obj_buffer = (ar_object_t *)objects;
    qsort(obj_buffer, objects_size, sizeof(ar_object_t), &compare_materials);
}

void ar_object_draw(ar_object_t *object, ar_shader_t *shader, camera_t *camera) {
    if (object->flags & AR_OBJECT_FLAG_UPDATE) {
        ar_object_update(object);
    }
    if (object->mesh != NULL) {
        mesh_draw(object->mesh, &object->matrix, camera, shader);
    }
}

void ar_objects_draw(ar_object_t *objects, int objects_size, ar_shader_t *shader, camera_t *camera, bool render_materials) {
    if (objects_sorted == false)
        ar_objects_sort(objects, objects_size);
    
    int i;
    hash_t mat_hash = 1;
    ar_object_t *object;
    for (i = 0; i < objects_size; i++) {
        object = &objects[i];
        if (render_materials == false) {
            material_update(NULL, shader);
        }
        else {
            if (mat_hash != get_material_hash(object)) {
                material_update(object->material, shader);
                mat_hash = get_material_hash(object);
            }        
        }
        ar_object_draw(object, shader, camera);
    }
}

void ar_objects_destroy(void) {
    ar_buffer_destroy(&object_buffer);
}
