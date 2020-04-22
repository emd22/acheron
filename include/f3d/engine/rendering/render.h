#ifndef F3D_RENDER_H
#define F3D_RENDER_H

#define MAX_RENDER_VIEWS 8
#define MAX_RENDER_OBJECTS 64

#define SCENE_ENABLE_SHADOWS 0x01
#define SCENE_ENABLE_SKYBOX  0x02

#include <f3d/engine/object/object.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/skybox.h>
#include <f3d/engine/rendering/camera.h>

enum {
    SCENE_VIEW,
    SCENE_SKYBOX,
};

extern shader_t *shader_main;
extern shader_t *shader_depth;

typedef struct {
    camera_t *camera;
    framebuffer_t framebuffer;
    vector2f_t dest_position, dest_dimensions;
    vector2f_t src_position, src_dimensions;
} render_view_t;

typedef struct {
    char name[32];

    render_view_t views[MAX_RENDER_VIEWS];
    int views_index;
    
    //object_t objects[MAX_RENDER_OBJECTS];
    //int objects_index;
    
    skybox_t skybox;
    int flags;
} scene_t;

scene_t *scene_new(const char *name);
render_view_t *scene_new_view(scene_t *scene, camera_t *camera, int width, int height, int attachment);
void scene_attach(scene_t *scene, int type, void *ptr);
void render_init();
int _scene_render(void *ptr);
void render_draw_scene(scene_t *scene);
void render_all();

#endif
