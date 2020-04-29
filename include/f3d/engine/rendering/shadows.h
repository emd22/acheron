#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>

extern camera_t shadow_cam;

typedef struct {
    int width, height;

    shader_t *shader;
    cubemap_t cubemap;
    mat4_t point_vps[6];
    framebuffer_t framebuffer;
    
    float far_plane;
    int shadow_map_id;
    
    mat4_t mat_perspective;
} shadows_point_t;

shadows_point_t shadows_point_init(vector3f_t position, int width, int height, float far_plane);
void shadows_point_render(shadows_point_t *shadow, vector3f_t position, shader_t *shader_main);
void shadows_point_update(shadows_point_t *shadow, vector3f_t position);

void shadows_render(shader_t *shader_main, camera_t *camera);
void shadows_destroy(shadows_point_t *shadow);

#endif
