#include <f3d/engine/object.h>
#include <f3d/engine/mesh.h>
#include <f3d/engine/types.h>

#include <f3d/engine/physics/object.h>

#include <string.h>

#define MAX_OBJECTS 256

static object_t objects[MAX_OBJECTS];
static int objects_index = 0;

object_t *object_new() {
    object_t *object = &objects[objects_index++];
    object->position = (vector3f_t){0, 0, 0};
    object->rotation = (vector3f_t){0, 0, 0};
    // set matrix to default, with no transformations
    // (1, 0, 0, 0)
    // (0, 1, 0, 0)
    // (0, 0, 1, 0)
    // (0, 0, 0, 1)
    object->scale = (vector3f_t){1, 1, 1};
    mat4_set(&object->matrix, MAT4_IDENTITY);
    object->mesh = NULL;
    glGenVertexArrays(1, &object->vao);
    return object;
}

void object_init(const char *name, object_t *object, int flags) {
    strcpy(object->name, name);
    
    object->flags = flags;
    
    // object has name string for easier debugging
    strcpy(object->name, name);

    object->physics.position = object->position;
    object->physics.collidable = true;
    object->physics.locked = false;
    object->physics.weight = 1.0f;
    object->physics.velocity = 0.0;
    object->physics.friction = 1.0f;
}

void object_scale(object_t *object) {
    //int i;
    mat4_t *mat = &object->matrix;
    // x
    mat->val[0] *= object->scale.x;
    mat->val[1] *= object->scale.x;
    mat->val[2] *= object->scale.x;
    mat->val[3] *= object->scale.x;
    
    mat->val[4] *= object->scale.y;
    mat->val[5] *= object->scale.y;
    mat->val[6] *= object->scale.y;
    mat->val[7] *= object->scale.y;
    
    mat->val[8] *= object->scale.z;
    mat->val[9] *= object->scale.z;
    mat->val[10] *= object->scale.z;
    mat->val[11] *= object->scale.z;
}

void object_attach_mesh(object_t *object, mesh_t *mesh) {
    glBindVertexArray(object->vao);
    object->mesh = mesh;
}

void object_update(object_t *object) {
    object->physics.position = object->position;
    
    // translations
    mat4_translate(&object->matrix, object->position);
    
    // do rotations
    object->matrix = mat4_rotate_x(object->matrix, object->rotation.x);
    object->matrix = mat4_rotate_y(object->matrix, object->rotation.y);
    object->matrix = mat4_rotate_z(object->matrix, object->rotation.z);
    object_scale(object);
    
}

void object_draw(object_t *object, camera_t *camera, shader_t *shader) {
    mesh_draw(object->mesh, &object->matrix, camera, shader);
}

bool object_check_collision(object_t *obj0, object_t *obj1) {
    return physics_check_collision_aabb(&obj0->physics, &obj1->physics);
}

void object_destroy(object_t *object) {
    glDeleteVertexArrays(1, &object->vao);
}

void objects_cleanup() {
    int i;
    for (i = 0; i < objects_index; i++)
        object_destroy(&objects[i]);
}
