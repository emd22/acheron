#ifndef F3D_CUBEMAP_H
#define F3D_CUBEMAP_H

#include <acheron/engine/image/texture.h>
#include <acheron/engine/renderer/rr_shader.h>
#include <acheron/engine/object/ob_mesh.h>

typedef struct {
    texture_t **textures;
    texture_t *map;
    
    ar_mesh_t *mesh;
    unsigned vao;
    unsigned vbo;
} cubemap_t;

void cubemap_init(cubemap_t *cubemap, texture_t **textures);
void ar_framebuffer_cubemap_init(cubemap_t *cubemap, int width, int height);

#endif
