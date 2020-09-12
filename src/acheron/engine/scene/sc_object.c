#include <acheron/engine/scene/sc_object.h>
#include <acheron/engine/object/material.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/util.h>
#include <acheron/engine/types.h>
#include <acheron/engine/limits.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static ar_buffer_t object_buffer;
static bool objects_sorted = false;

void ar_objects_sort(ar_object_t *objects, int objects_size);

ar_object_t *ar_object_new(const char *name) {
    if (!ar_buffer_is_initialized(&object_buffer)) {
        ar_buffer_init(&object_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_object_t), DEFAULT_OBJECTS, 0);
    }
    ar_object_t *object = ar_buffer_new_item(&object_buffer);
    
    strcpy(object->name, name);
    object->hash = util_hash_str(name);
    object->mesh = NULL;
    object->position = (ar_vector3f_t){0, 0, 0};
    object->rotation = (ar_vector3f_t){0, 0, 0};
    object->scale = (ar_vector3f_t){1, 1, 1};
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

static void scale_object(ar_object_t *object) {
    // lazy way of mat3(matrix)*(x,y,z)
    object->matrix = ar_mat4_mul_vec4(
        object->matrix, 
        (ar_vector4f_t){
            object->scale.x, 
            object->scale.y, 
            object->scale.z,
            1.0 /* Don't modify translations */
        }
    );
}

void ar_object_move_v(ar_object_t *object, ar_vector3f_t val) {
    object->position = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}

void ar_object_rotate_v(ar_object_t *object, ar_vector3f_t val) {
    object->rotation = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}
void ar_object_scale_v(ar_object_t *object, ar_vector3f_t val) {
    object->scale = val;
    object->flags |= AR_OBJECT_FLAG_UPDATE;
}

void ar_object_move(ar_object_t *object, float x, float y, float z) {
    ar_object_move_v(object, (ar_vector3f_t){x, y, z});
}

void ar_object_rotate(ar_object_t *object, float x, float y, float z) {
    ar_object_rotate_v(object, (ar_vector3f_t){x, y, z});
}

void ar_object_scale(ar_object_t *object, float x, float y, float z) {
    ar_object_scale_v(object, (ar_vector3f_t){x, y, z});
}

void ar_object_update(ar_object_t *object) {
    // translations
    ar_mat4_translate(&object->matrix, object->position);
    ar_quat_from_euler(&object->quat_rotation, object->rotation);

    scale_object(object);
    object->physics.collider.position = object->position;
    object->physics.collider.scale = object->scale;
}

void ar_object_attach(ar_object_t *object, int type, void *data) {
    if (type == AR_OBJECT_ATTACH_MESH) {
        object->mesh = (ar_asset_t *)data;
        //physics_collider_stretch_to_vertices(&object->physics.collider, &object->mesh->vertices);
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

void ar_object_draw(ar_object_t *object, ar_shader_t *shader, ar_camera_t *camera) {
    if (object->flags & AR_OBJECT_FLAG_UPDATE) {
        ar_object_update(object);
    }
    if (object->mesh != NULL) {
        if (object->mesh->status == AR_ASSET_IN_MEM)
            ar_asset_resolve(object->mesh);
        ar_mesh_draw(ar_asset_get_mesh(object->mesh), &object->matrix, &object->quat_rotation, camera, shader);
    }
}

void ar_objects_draw(ar_object_t *objects, int objects_size, ar_shader_t *shader, ar_camera_t *camera, bool render_materials) {
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

void ar_object_buffer_destroy(void) {
    ar_buffer_destroy(&object_buffer);
}
