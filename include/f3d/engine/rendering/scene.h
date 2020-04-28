#ifndef F3D_SCENE_H
#define F3D_SCENE_H

#include <f3d/engine/rendering/render_view.h>
#include <f3d/engine/rendering/skybox.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/light.h>
#include <f3d/engine/rendering/shadows.h>

#define SCENE_MAX_RENDER_VIEWS 8
#define SCENE_MAX_LIGHTS       8

#define SCENE_ENABLE_SHADOWS 0x01
#define SCENE_ENABLE_SKYBOX  0x02

enum {
    SCENE_SKYBOX,
    SCENE_LIGHT,
};

typedef struct {
    char name[32];

    render_view_t views[SCENE_MAX_RENDER_VIEWS];
    int views_index;
    
    //object_t objects[MAX_RENDER_OBJECTS];
    //int objects_index;
    
    light_t *lights[SCENE_MAX_LIGHTS];
    int lights_index;
    
    skybox_t skybox;
    int flags;
} scene_t;

extern scene_t scenes[8];
extern int scenes_index;

scene_t *scene_new(const char *name);
render_view_t *scene_new_view(scene_t *scene, camera_t *camera, int width, int height, int attachment);
void scene_attach(scene_t *scene, int type, void *ptr);
void scene_render(shader_t *shader_main, scene_t *scene);
void scene_destroy(scene_t *scene);

#endif
