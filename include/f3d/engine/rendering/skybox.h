#ifndef F3D_SKYBOX_H
#define F3D_SKYBOX_H

#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/model/mesh.h>

typedef struct {
    cubemap_t cubemap;
    mesh_t *mesh;
} skybox_t;

skybox_t skybox_new(texture_t **textures);
void skybox_render(skybox_t *skybox, camera_t *camera);

#endif
