#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <f3d/engine/renderer/rr_shaderman.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/physics/object.h>

extern camera_t shadow_cam;

typedef struct {
    int width, height;

    ar_shader_t *shader;
    camera_t camera;
    cubemap_t cubemap;
    mat4_t point_vps[6];
    ar_framebuffer_t framebuffer;
    
    float far_plane;
    int shadow_map_id;
    
    physics_collider_t collider;
    
    mat4_t mat_perspective;
} shadows_point_t;

shadows_point_t shadows_point_init(ar_vector3f_t position, int width, int height, float far_plane);
void shadows_point_render(shadows_point_t *shadow, ar_vector3f_t position, ar_shader_t *shader_main);
void shadows_point_update(shadows_point_t *shadow, ar_vector3f_t position);

void shadows_render(ar_shader_t *shader_main, camera_t *camera);
void shadows_destroy(shadows_point_t *shadow);

#endif
