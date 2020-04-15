#ifndef F3D_CUBEMAP_H
#define F3D_CUBEMAP_H

#include <f3d/engine/texture.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/camera.h>

typedef struct {
    texture_t **textures;
    texture_t *map;
    
    unsigned vao;
    unsigned vbo;
} cubemap_t;

void cubemap_init(cubemap_t *cubemap, texture_t **textures);
void cubemap_render(cubemap_t *cubemap, shader_t *skybox_shader, camera_t *camera);


#endif
