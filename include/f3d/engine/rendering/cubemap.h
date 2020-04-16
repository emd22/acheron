#ifndef F3D_CUBEMAP_H
#define F3D_CUBEMAP_H

#include <f3d/engine/image/texture.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/camera.h>

typedef struct {
    texture_t **textures;
    texture_t *map;
    
    unsigned vao;
    unsigned vbo;
} cubemap_t;

void cubemaps_init();
void cubemap_init(cubemap_t *cubemap, texture_t **textures);
void cubemap_render(cubemap_t *cubemap, camera_t *camera);


#endif
