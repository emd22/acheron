#ifndef F3D_CUBEMAP_H
#define F3D_CUBEMAP_H

#include <f3d/engine/image/texture.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/model/mesh.h>

typedef struct {
    texture_t **textures;
    texture_t *map;
    
    mesh_t *mesh;
    unsigned vao;
    unsigned vbo;
} cubemap_t;

void cubemap_init(cubemap_t *cubemap, texture_t **textures);
void framebuffer_cubemap_init(cubemap_t *cubemap, int width, int height);

#endif
