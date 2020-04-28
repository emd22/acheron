#ifndef F3D_RENDER_H
#define F3D_RENDER_H


#include <f3d/engine/object/object.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/scene.h>

#define MAX_RENDER_OBJECTS 64

extern shader_t *shader_main;
extern scene_t *selected_scene;

void render_init_shadows(scene_t *scene, int width, int height);
void render_init();
void render_draw_scene(scene_t *scene);
void render_all();
void render_destroy(void);

#endif
