#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/light.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>

extern framebuffer_t shadow_fb;
extern camera_t shadow_cam;
extern shader_t *shader_depth;

void shadows_init(int width, int height, vector3f_t direction, vector3f_t center);
void shadows_update(vector3f_t direction, vector3f_t center);
void shadows_render(shader_t *shader_main);
void shadows_destroy();

#endif
