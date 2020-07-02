#ifndef F3D_SKYBOX_H
#define F3D_SKYBOX_H

#include <acheron/engine/rendering/camera.h>
#include <acheron/engine/rendering/cubemap.h>
#include <acheron/engine/object/ob_mesh.h>

typedef struct {
    cubemap_t cubemap;
    ar_mesh_t *mesh;
} skybox_t;

skybox_t skybox_new(texture_t **textures);
void skybox_render(skybox_t *skybox, camera_t *camera);

#endif
