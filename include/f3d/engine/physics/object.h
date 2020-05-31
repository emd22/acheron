#ifndef F3D_PHYSICS_OBJ_H
#define F3D_PHYSICS_OBJ_H

#include <f3d/engine/types.h>
#include <f3d/engine/model/mesh.h>
#include <f3d/engine/rendering/shader.h>
#include <stdbool.h>

typedef enum {
    PHYSICS_COLLIDER_AABB,
} physics_collider_type_t;

typedef struct {
    physics_collider_type_t type;
    
    vector3f_t position;
    vector3f_t dimensions;
    vector3f_t scale;
    
    mesh_t *debug_mesh;
} physics_collider_t;

typedef struct {
    physics_collider_t collider;
    vector3f_t velocity;
    vector3f_t rotation;
    vector3f_t torque;
    
    float restitution;
    float mass;
    float friction;
    
    bool locked;
} physics_object_t;

extern shader_t *shader_bounding;

physics_object_t physics_object_new(physics_collider_type_t collider_type);
physics_collider_t physics_collider_new(physics_collider_type_t collider_type);
void physics_collider_stretch_to_vertices(physics_collider_t *collider, ar_buffer_t *vertices);
bool physics_collider_check_collision(physics_collider_t *collider0, physics_collider_t *collider1);
bool physics_check_collision(physics_object_t *object0, physics_object_t *object1);
void physics_update_gravity(physics_object_t *obj);

bool physics_check_collision_aabb(physics_object_t *obj0, physics_object_t *obj1);
bool physics_update(physics_object_t *obj, physics_object_t *ground);
#endif
