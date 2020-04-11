#include <f3d/engine/physics/object.h>

#include <math.h>

#define OBJ_MAX_X(obj) (obj->position.x+obj->dimensions.x)
#define OBJ_MAX_Y(obj) (obj->position.y+obj->dimensions.y)
#define OBJ_MAX_Z(obj) (obj->position.z+obj->dimensions.z)

#define GRAVITY           0.15f
#define TERMINAL_VELOCITY 0.6f

bool physics_check_collision_aabb(physics_object_t *obj0, physics_object_t *obj1) {
    const float maxx0 = OBJ_MAX_X(obj0),  maxx1 = OBJ_MAX_X(obj1);
    const float minx0 = obj0->position.x, minx1 = obj1->position.x;
    
    const float maxy0 = OBJ_MAX_Y(obj0),  maxy1 = OBJ_MAX_Y(obj1);
    const float miny0 = obj0->position.y, miny1 = obj1->position.y;
    
    const float maxz0 = OBJ_MAX_Z(obj0),  maxz1 = OBJ_MAX_Z(obj1);
    const float minz0 = obj0->position.z, minz1 = obj1->position.z;
    
    if ((minx0 <= maxx1 && maxx0 >= minx1) &&
        (miny0 <= maxy1 && maxy0 >= miny1) &&
        (minz0 <= maxz1 && maxz0 >= minz1)) 
    {
        return true;
    }
    
    return false;   
}

void physics_move(physics_object_t *obj, vector3f_t direction) {
    (void)obj;
    (void)direction;
}

void physics_update(physics_object_t *obj) {
    if (obj->locked || obj->weight == 0)
        return;
}
