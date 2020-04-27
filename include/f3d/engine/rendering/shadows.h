#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/light.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>

extern framebuffer_t shadow_fb;
extern camera_t shadow_cam;
extern shader_t *shader_depth;
extern cubemap_t shadow_cubemap;

void shadows_init(light_t *light, int width, int height);
void shadows_update(light_t *light, int width, int height);
void shadows_render(shader_t *shader_main, camera_t *camera);
void shadows_destroy();

#endif
