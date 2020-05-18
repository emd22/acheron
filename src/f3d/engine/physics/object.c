#include <f3d/engine/physics/object.h>
#include <f3d/engine/rendering/render.h>
#include <f3d/engine/core/log.h>

#include <math.h>

#define OBJ_MAX_X(obj) (obj->position.x+obj->dimensions.x)
#define OBJ_MAX_Y(obj) (obj->position.y+obj->dimensions.y)
#define OBJ_MAX_Z(obj) (obj->position.z+obj->dimensions.z)

shader_t *shader_bounding = NULL;

physics_collider_t physics_collider_new(physics_collider_type_t type) {
    physics_collider_t collider;
    collider.type = type;
    collider.position = (vector3f_t){0, 0, 0};
    collider.dimensions = (vector3f_t){1, 1, 1};
    
    if (shader_bounding == NULL) {
        shader_bounding = shader_new("BoundingBox");
        shader_attach(shader_bounding, SHADER_VERTEX, "../shaders/debug/bounding_vert.glsl");
        shader_attach(shader_bounding, SHADER_FRAGMENT, "../shaders/debug/bounding_frag.glsl");
    }
    
    collider.debug_mesh = mesh_new();
    return collider;
}

static bool collider_check_aabb(physics_collider_t *obj0, physics_collider_t *obj1) {
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

void physics_collider_stretch_to_vertices(physics_collider_t *collider, buffer_t *vertices) {
    vector2f_t width, height, depth;
    width =  (vector2f_t){0.0f, 0.0f};
    height = (vector2f_t){0.0f, 0.0f};
    depth =  (vector2f_t){0.0f, 0.0f};
    unsigned i;
    vector3f_t vertex;
    for (i = 0; i < vertices->index; i++) {
        vertex = (((vertex_t *)vertices->data)[i]).position;
        // x
        // find smallest x value in vertex
        if (vertex.x < 0 && vertex.x < width.x)
            width.x = vertex.x;
        // find largest x value in vertex
        else if (vertex.x >= 0 && vertex.x > width.y)
            width.y = vertex.x;
            
        // y
        // find smallest y value in vertex
        if (vertex.y < 0 && vertex.y < height.x)
            height.x = vertex.y;
        // find largest y value in vertex
        else if (vertex.y >= 0 && vertex.y > height.y)
            height.y = vertex.y;
            
        // z
        // find smallest z value in vertex
        if (vertex.z < 0 && vertex.z < depth.x)
            depth.x = vertex.z;
        // find largest z value in vertex
        else if (vertex.z >= 0 && vertex.z > depth.y)
            depth.y = vertex.z;
    }
    float pwidth = width.y-width.x;
    float pheight = height.y-height.x;
    float pdepth = depth.y-depth.x;
    collider->dimensions = (vector3f_t){pwidth, pheight, pdepth};
    
    vector3f_t pointsp[] = {
        (vector3f_t){width.x, width.y, height.x},
        (vector3f_t){height.y, depth.x, depth.y}
    };
    buffer_t points = buffer_from_data(BUFFER_STATIC, pointsp, sizeof(vector3f_t), 6);
    mesh_set_data(collider->debug_mesh, &points, NULL, NULL);
}

bool physics_collider_check_collision(physics_collider_t *collider0, physics_collider_t *collider1) {
    if (collider0->type == PHYSICS_COLLIDER_AABB && collider1->type == PHYSICS_COLLIDER_AABB) {
        return collider_check_aabb(collider0, collider1);
    }
    else {
        log_msg(LOG_WARN, "Not implemented\n", 0);
    }
    return false;
}

void physics_move(physics_object_t *obj, vector3f_t direction) {
    (void)obj;
    (void)direction;
}

void physics_update(physics_object_t *obj) {
    (void)obj;
    return;
}
