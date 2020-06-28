#ifndef F3D_OBJECT_H
#define F3D_OBJECT_H

#include <f3d/engine/types.h>
#include <f3d/engine/util.h>
#include <f3d/engine/model/mesh.h>
#include <f3d/engine/renderer/rr_shader.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/physics/object.h>

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

    mesh_t *mesh;
    material_t *material;
    
    ar_vector3f_t position;
    ar_vector3f_t rotation;
    ar_vector3f_t scale;
    mat4_t matrix;
    
    physics_object_t physics;
    
    int flags;
} ar_object_t;

ar_object_t *ar_object_new(const char *name);
void render_set_target(int target, void *ptr);
void ar_object_update(ar_object_t *object);

void object_move(ar_object_t *object, float x, float y, float z);
void object_move_v(ar_object_t *object, ar_vector3f_t val);
void object_rotate(ar_object_t *object, float x, float y, float z);
void object_rotate_v(ar_object_t *object, ar_vector3f_t val);
void object_scale(ar_object_t *object, float x, float y, float z);
void object_scale_v(ar_object_t *object, ar_vector3f_t val);

ar_object_t *ar_object_get(const char *name);
void ar_objects_sort(ar_object_t *objects, int objects_size);
void ar_objects_draw(ar_object_t *objects, int objects_size, ar_shader_t *shader, camera_t *camera, bool render_materials);
void ar_object_attach(ar_object_t *object, int type, void *data);
void ar_object_draw(ar_object_t *object, ar_shader_t *shader, camera_t *camera);
void ar_object_buffer_destroy(void);

#endif
