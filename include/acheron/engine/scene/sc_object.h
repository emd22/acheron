#ifndef F3D_OBJECT_H
#define F3D_OBJECT_H

#include <acheron/engine/types.h>
#include <acheron/engine/util.h>
#include <acheron/engine/object/ob_mesh.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/scene/sc_camera.h>
#include <acheron/engine/rendering/framebuffer.h>
#include <acheron/engine/object/material.h>
#include <acheron/engine/physics/object.h>
#include <acheron/engine/asset/at_asset.h>

#include <acheron/engine/math/mt_quat.h>

#include <stdbool.h>

#define AR_OBJECT_FLAG_UPDATE 0x01

typedef enum {
    AR_RENDER_TARGET_FRAMEBUFFER,
    AR_RENDER_TARGET_CUBEMAP,
} ar_object_render_target_t;

typedef enum {
    AR_OBJECT_ATTACH_MESH,
    AR_OBJECT_ATTACH_MATERIAL,
} ar_object_attach_type_t;

typedef enum {
    AR_OBJECT_TYPE_MESH,
} ar_object_type_t;

typedef enum {
    AR_OBJECT_UPDATE_NEARBY,
} ar_object_update_type_t;

typedef struct object_s {
    char name[32];
    hash_t hash;

    //ar_mesh_t *mesh;
    ar_asset_t *mesh;
    material_t *material;
    
    ar_vector3f_t position;
    ar_vector3f_t rotation;
    ar_vector3f_t scale;
    
    ar_mat4_t matrix;
    ar_quat_t quat_rotation;
    
    physics_object_t physics;
    
    int flags;
} ar_object_t;

ar_object_t *ar_object_new(const char *name);
void render_set_target(int target, void *ptr);
void ar_object_update(ar_object_t *object);

void ar_object_move(ar_object_t *object, float x, float y, float z);
void ar_object_move_v(ar_object_t *object, ar_vector3f_t val);
void ar_object_rotate(ar_object_t *object, float x, float y, float z);
void ar_object_rotate_v(ar_object_t *object, ar_vector3f_t val);
void ar_object_scale(ar_object_t *object, float x, float y, float z);
void ar_object_scale_v(ar_object_t *object, ar_vector3f_t val);

ar_object_t *ar_object_get(const char *name);
void ar_objects_sort(ar_object_t *objects, int objects_size);
void ar_objects_draw(ar_object_t *objects, int objects_size, ar_shader_t *shader, ar_camera_t *camera, bool render_materials);
void ar_object_attach(ar_object_t *object, int type, void *data);
void ar_object_draw(ar_object_t *object, ar_shader_t *shader, ar_camera_t *camera);
void ar_object_buffer_destroy(void);

#endif
