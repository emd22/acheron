#include <acheron/engine/physics/object.h>
#include <acheron/engine/rendering/render.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_time.h>

#include <math.h>

#define OBJ_MAX_X(obj) (obj->position.x+(obj->dimensions.x))*obj->scale.x
#define OBJ_MAX_Y(obj) (obj->position.y+(obj->dimensions.y))*obj->scale.y
#define OBJ_MAX_Z(obj) (obj->position.z+(obj->dimensions.z))*obj->scale.z

ar_shader_t *shader_bounding = NULL;

physics_collider_t physics_collider_new(physics_collider_type_t type) {
    physics_collider_t collider;
    collider.type = type;
    collider.position = (ar_vector3f_t){0, 0, 0};
    collider.dimensions = (ar_vector3f_t){1, 1, 1};
    //collider.rotation = (vector3f_t){0, 0, 0};
    collider.scale = (ar_vector3f_t){1, 1, 1};
    
    if (shader_bounding == NULL) {
        shader_bounding = ar_shader_new("BoundingBox");
        ar_shader_attach(shader_bounding, AR_SHADER_VERTEX, "../shaders/debug/bounding_vert.glsl");
        ar_shader_attach(shader_bounding, AR_SHADER_FRAGMENT, "../shaders/debug/bounding_frag.glsl");
    }
    return collider;
}

physics_object_t physics_object_new(physics_collider_type_t collider_type) {
    physics_object_t object;
    object.collider = physics_collider_new(collider_type);
    object.velocity = (ar_vector3f_t){0, 0, 0};
    object.torque = (ar_vector3f_t){0, 0, 0};
    object.locked = false;
    object.mass = 10.0f;
    object.friction = 1.0f;
    return object;
}

static bool collider_check_aabb(physics_collider_t *obj0, physics_collider_t *obj1) {
    const float maxx0 = OBJ_MAX_X(obj0),  maxx1 = OBJ_MAX_X(obj1);
    const float minx0 = obj0->position.x, minx1 = obj1->position.x;
    
    const float maxy0 = OBJ_MAX_Y(obj0),  maxy1 = OBJ_MAX_Y(obj1);
    const float miny0 = obj0->position.y, miny1 = obj1->position.y;
    
    const float maxz0 = OBJ_MAX_Z(obj0),  maxz1 = OBJ_MAX_Z(obj1);
    const float minz0 = obj0->position.z, minz1 = obj1->position.z;
    
    if (minx0 <= maxx1 && maxx0 >= minx1 &&
        miny0 <= maxy1 && maxy0 >= miny1 &&
        minz0 <= maxz1 && maxz0 >= minz1) 
        return true;
    
    return false;
}

void physics_collider_stretch_to_vertices(physics_collider_t *collider, ar_buffer_t *vertices) {
    ar_vector2f_t width, height, depth;
    width =  (ar_vector2f_t){0.0f, 0.0f};
    height = (ar_vector2f_t){0.0f, 0.0f};
    depth =  (ar_vector2f_t){0.0f, 0.0f};
    unsigned i;
    ar_vector3f_t vertex;
    for (i = 0; i < vertices->index; i++) {
        vertex = (((vertex_t *)vertices->data)[i]).position;
        // x
        // find smallest x value in vertex
        if (vertex.x < width.x)
            width.x = vertex.x;
        // find largest x value in vertex
        else if (vertex.x > width.y)
            width.y = vertex.x;
            
        // y
        // find smallest y value in vertex
        if (vertex.y < height.x)
            height.x = vertex.y;
        // find largest y value in vertex
        else if (vertex.y > height.y)
            height.y = vertex.y;
            
        // z
        // find smallest z value in vertex
        if (vertex.z < depth.x)
            depth.x = vertex.z;
        // find largest z value in vertex
        else if (vertex.z > depth.y)
            depth.y = vertex.z;
    }
    float pwidth = width.y-width.x;
    float pheight = height.y-height.x;
    float pdepth = depth.y-depth.x;
    collider->position = (ar_vector3f_t){width.x, height.x, depth.y};
    collider->dimensions = (ar_vector3f_t){pwidth, pheight, pdepth};
}

bool physics_collider_check_collision(physics_collider_t *collider0, physics_collider_t *collider1) {
    if (collider0->type == PHYSICS_COLLIDER_AABB && collider1->type == PHYSICS_COLLIDER_AABB) {
        return collider_check_aabb(collider0, collider1);
    }
    else {
        ar_log(AR_LOG_WARN, "Not implemented\n", 0);
    }
    return false;
}

void physics_move(physics_object_t *obj, ar_vector3f_t direction) {
    (void)obj;
    (void)direction;
}

bool physics_check_collision(physics_object_t *object0, physics_object_t *object1) {
    return physics_collider_check_collision(&object0->collider, &object1->collider);
}

void linear_collision_impulse(physics_object_t *obj, physics_object_t *ground) {
    (void)ground;
    if (obj->locked)
        return;
        
    // TODO: replace
    // set position to be above ground
    obj->collider.position.y = ground->collider.position.y+ground->collider.dimensions.y;
    /*
    float mass = obj->mass;
    
    vec3f_t rv;
    vec3f_sub(&rv, obj.velocity);
    float contact_vel = vec3f_dot(rv, normal);
    float e = fmin(obj->resititution, ground->restitution);
    
    float j = -(1.0f+e)*contact_vel;
    j /= obj->mass+b->mass;
    */
    
    //const float relx = (obj->collider.position.x < ground->collider.position.x) ? -1.0f : 1.0f;
    //const float rely = (obj->collider.position.y < ground->collider.position.y) ? -1.0f : 1.0f;
    //const float relz = (obj->collider.position.z < ground->collider.position.z) ? -1.0f : 1.0f;
    
    float reflecty = 2*1*(obj->velocity.y);
    obj->velocity.y -= reflecty*0.8f;
    //obj->velocity.x *= 0.98f;
    obj->velocity.z *= 0.98f;
    
    if (reflecty-2.0f < 0.4f)
        obj->locked = true;
    
    //vector3f_t normal;
    //if (obj->collider.normals[0])
    //    normal = (vector3f_t){relx, 0, 0};
    //else if (obj->collider.normals[1])
    //    normal = (vector3f_t){0, rely, 0};
    //else if (obj->collider.normals[2])
    //    normal = (vector3f_t){0, 0, relz};
    
    //vector3f_t rv;
    //vec3f_sub(&rv, obj->velocity, ground->velocity);
    //float cv = vec3f_dot(rv, normal);
    
    //if (cv > 0) {
    //    log_msg(LOG_INFO, "Contact > 0\n", 0);
    //    return;
    //}
    //float e = fmin(obj->restitution, ground->resititution);
    //float j = -(1.0f+e)*cv;
    //j /= ob->
    
}

void physics_update_gravity(physics_object_t *obj) {
    if (obj->locked)
        return;
    const float delta_time = ar_time_get_delta();
    float velocityy = -(obj->velocity.y * delta_time)+(0.5f*9.81f*delta_time*delta_time);
    obj->collider.position.x += obj->velocity.x;
    obj->collider.position.y += velocityy;
    obj->collider.position.z += obj->velocity.z;
    obj->velocity.y += (9.81f*delta_time);
    
    
    //vector3f_t rotvec = (vector3f_t){0.5*delta_time*obj->velocity.x, 0.5*delta_time*obj->velocity.y, 0.5*delta_time*obj->velocity.z};
    //vec3f_add(&obj->collider.rotation, obj->collider.rotation, rotvec);
}

bool physics_update(physics_object_t *obj, physics_object_t *ground) {
    physics_update_gravity(obj);
    if (physics_check_collision(obj, ground)) {
        linear_collision_impulse(obj, ground);
    }

    return false;
}
