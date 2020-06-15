#ifndef F3D_RENDER_H
#define F3D_RENDER_H


#include <f3d/engine/renderer/rr_shader.h>
#include <f3d/engine/scene/object.h>
#include <f3d/engine/scene/scene.h>

extern ar_shader_t *shader_main;

void render_init_shadows(ar_scene_t *scene, int width, int height);
void render_init();
void render_draw_scene(ar_scene_t *scene);
void render_all();
void render_destroy(void);

#endif
