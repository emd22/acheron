#ifndef AR_SCENE_H
#define AR_SCENE_H

#include <f3d/engine/scene/scene.h>

#include <f3d/engine/limits.h>
#include <f3d/engine/rendering/render_view.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/scene/light.h>
#include <f3d/engine/scene/skybox.h>
#include <f3d/engine/scene/object.h>
#include <f3d/engine/types.h>

#define SCENE_ENABLE_SHADOWS 0x01
#define SCENE_ENABLE_SKYBOX  0x02

typedef enum {
    SCENE_SKYBOX,
    SCENE_LIGHT,
    SCENE_OBJECT,
} ar_scene_attach_type_t;

typedef struct {
    char name[32];
    
    ar_object_buffer_t objects;
    ar_buffer_t lights;
    
    //skybox_t skybox;
    int flags;
} ar_scene_t;

/*typedef struct {
    char name[32];
    
    ar_buffer_t objects;
    ar_buffer_t lights;
} ar_scene_t;*/

extern ar_buffer_t scenes;

ar_scene_t *ar_scene_new(const char *name);
void ar_scene_render_shadows(ar_scene_t *scene, ar_shader_t *shader_main);
void ar_scene_objects_render(ar_scene_t *scene, ar_shader_t *shader, camera_t *camera, bool render_materials);
void ar_scene_select(ar_scene_t *scene, ar_shader_t *shader_main);
ar_scene_t *ar_scene_get_selected(void);
render_view_t *ar_scene_new_view(ar_scene_t *scene, camera_t *camera, int width, int height, int attachment);
void ar_scene_object_update(ar_scene_t *scene, object_t *object, ar_shader_t *shader_main);
void *ar_scene_attach(ar_scene_t *scene, ar_scene_attach_type_t type, void *ptr);
void ar_scene_render(ar_shader_t *shader_main, ar_scene_t *scene);
void ar_scene_destroy(ar_scene_t *scene);

#endif
