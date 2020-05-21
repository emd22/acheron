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

#define RENDER_OBJECT_FLAG_UPDATE 0x01

enum {
    RENDER_TARGET_FRAMEBUFFER,
    RENDER_TARGET_CUBEMAP,
};

enum {
    OBJECT_ATTACH_MESH,
    OBJECT_ATTACH_MATERIAL,
};

enum {
    RENDER_OBJECT_TYPE_MESH,
};

typedef enum {
    OBJECT_UPDATE_NEARBY,
} object_update_type_t;

typedef struct render_object_s {
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
} render_object_t;

render_object_t *object_new(const char *name);
void render_set_target(int target, void *ptr);
void object_update(render_object_t *object);

void object_move(render_object_t *object, float x, float y, float z);
void object_move_v(render_object_t *object, vector3f_t val);
void object_rotate(render_object_t *object, float x, float y, float z);
void object_rotate_v(render_object_t *object, vector3f_t val);
void object_scale(render_object_t *object, float x, float y, float z);
void object_scale_v(render_object_t *object, vector3f_t val);

render_object_t *object_get(const char *name);
void objects_sort(void);
void object_attach(render_object_t *object, int type, void *data);
void objects_draw(shader_t *shader, camera_t *camera, bool render_materials);
void object_draw(render_object_t *object, shader_t *shader, camera_t *camera);

#endif
