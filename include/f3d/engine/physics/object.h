#ifndef F3D_PHYSICS_OBJ_H
#define F3D_PHYSICS_OBJ_H

#include <f3d/engine/types.h>
#include <stdbool.h>

typedef struct {
    vector3f_t position;
    vector3f_t dimensions;
    
    bool collidable;
    bool locked;
    
    float velocity;
    float weight;
    float friction;
} physics_object_t;

bool physics_check_collision_aabb(physics_object_t *obj0, physics_object_t *obj1);
void physics_update(physics_object_t *obj);

#endif
