#ifndef F3D_OBJECT_H
#define F3D_OBJECT_H

#include <f3d/engine/types.h>
#include <f3d/engine/util.h>
#include <f3d/engine/model/mesh.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/physics/object.h>

#include <stdbool.h>

#define RENDER_OBJECT_FLAG_UPDATE 0x01

enum {
    AR_RENDER_TARGET_FRAMEBUFFER,
    AR_RENDER_TARGET_CUBEMAP,
};

enum {
    AR_OBJECT_ATTACH_MESH,
    AR_OBJECT_ATTACH_MATERIAL,
};

enum {
    AR_RENDER_OBJECT_TYPE_MESH,
};

typedef enum {
    AR_OBJECT_UPDATE_NEARBY,
} ar_object_update_type_t;

typedef struct {
    ar_buffer_t buffer;
    bool sorted;
} ar_object_buffer_t;

typedef struct object_s {
    char name[32];
    hash_t hash;

    mesh_t *mesh;
    material_t *material;
    
    vector3f_t position;
    vector3f_t rotation;
    vector3f_t scale;
    mat4_t matrix;
    
    physics_object_t physics;
    
    int flags;
} object_t;

object_t object_new(const char *name);
ar_object_buffer_t object_buffer_new(ar_buffer_type_t buffer_type, int buffer_start_size);
void render_set_target(int target, void *ptr);
void object_update(object_t *object);

void object_move(object_t *object, float x, float y, float z);
void object_move_v(object_t *object, vector3f_t val);
void object_rotate(object_t *object, float x, float y, float z);
void object_rotate_v(object_t *object, vector3f_t val);
void object_scale(object_t *object, float x, float y, float z);
void object_scale_v(object_t *object, vector3f_t val);

object_t *object_get(const char *name);
void objects_sort(ar_object_buffer_t *objects);
void object_attach(object_t *object, int type, void *data);
void objects_draw(ar_object_buffer_t *objects, ar_shader_t *shader, camera_t *camera, bool render_materials);
void object_draw(object_t *object, ar_shader_t *shader, camera_t *camera);

#endif
