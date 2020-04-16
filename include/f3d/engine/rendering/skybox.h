#ifndef F3D_SKYBOX_H
#define F3D_SKYBOX_H

#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/cubemap.h>

void skybox_init(cubemap_t *cubemap);
void skybox_render(camera_t *camera);

#endif
