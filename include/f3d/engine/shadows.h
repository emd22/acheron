#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <f3d/engine/shader.h>
#include <f3d/engine/light.h>
#include <f3d/engine/camera.h>
#include <f3d/engine/framebuffer.h>

extern framebuffer_t shadow_fb;

void shadows_init(int width, int height, light_t *light);
void shadows_render(shader_t *shader_main, shader_t *shader_depth);

#endif
