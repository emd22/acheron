#ifndef F3D_SHADOWS_H
#define F3D_SHADOWS_H

#include <acheron/engine/renderer/rr_shaderman.h>
#include <acheron/engine/camera/cm_perspective.h>
#include <acheron/engine/rendering/framebuffer.h>
#include <acheron/engine/rendering/cubemap.h>
#include <acheron/engine/physics/object.h>

extern ar_camera_t shadow_cam;

typedef struct {
    int width, height;

    ar_shader_t *shader;
    ar_camera_perspective_t camera;
    cubemap_t cubemap;
    ar_mat4_t point_vps[6];
    ar_framebuffer_t framebuffer;
    
    float far_plane;
    int shadow_map_id;
    
    physics_collider_t collider;
    
    ar_mat4_t mat_perspective;
} shadows_point_t;

shadows_point_t shadows_point_init(ar_vector3f_t position, int width, int height, float far_plane);
void shadows_point_render(shadows_point_t *shadow, ar_vector3f_t position, ar_shader_t *shader_main);
void shadows_point_update(shadows_point_t *shadow, ar_vector3f_t position);

void shadows_render(ar_shader_t *shader_main, ar_camera_t *camera);
void shadows_destroy(shadows_point_t *shadow);

#endif
