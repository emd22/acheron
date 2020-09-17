#ifndef F3D_RENDER_H
#define F3D_RENDER_H


#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/scene/sc_object.h>
#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/scene/sc_camera.h>

void render_init_shadows(ar_scene_t *scene, int width, int height);
void render_init();
void render_draw_scene(ar_scene_t *scene);
void render_all(ar_camera_t *camera);
void render_destroy(void);

#endif
