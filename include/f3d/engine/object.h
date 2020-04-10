#ifndef F3D_OBJECT_H
#define F3D_OBJECT_H

#include <f3d/engine/physics/object.h>
#include <f3d/engine/mesh.h>

typedef struct {
    char name[16];
    int flags;
    physics_object_t physics;

    mesh_t *mesh;
    vector3f_t position, rotation;
    mat4_t matrix;
} object_t;

object_t *object_new();
void object_init(const char *name, object_t *object, int flags);
void object_attach_mesh(object_t *object, mesh_t *mesh);
void object_draw(object_t *object, camera_t *camera, shader_t *shader);
void object_update(object_t *object);
bool object_check_collision(object_t *obj0, object_t *obj1);
void object_destroy(object_t *object);
void objects_cleanup();

#endif
