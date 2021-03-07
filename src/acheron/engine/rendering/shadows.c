#include <acheron/engine/rendering/shadows.h>
#include <acheron/engine/rendering/framebuffer.h>
#include <acheron/engine/rendering/cubemap.h>

#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/math/mt_math.h>

#include <acheron/engine/scene/sc_scene.h>
#include <acheron/engine/scene/sc_object.h>
#include <acheron/engine/types.h>

#include <stdio.h>

static ar_shader_t *shader_point_shadow = NULL;

void generate_point_vp(int index, ar_vector3f_t position, shadows_point_t *shadow, ar_vector3f_t offset, ar_vector3f_t upvec) {
    ar_vector3f_t to;
    ar_vector_add(AR_VEC3F, &position, &offset, &to);
    shadow->point_vps[index] = ar_math_lookat(position, to, upvec);
    shadow->point_vps[index] = ar_mat4_mul(shadow->mat_perspective, shadow->point_vps[index]);
}

void generate_point_vps(shadows_point_t *shadow, ar_vector3f_t position) {
    generate_point_vp(0, position, shadow, (ar_vector3f_t){ 1.0f,  0.0f,  0.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(1, position, shadow, (ar_vector3f_t){-1.0f,  0.0f,  0.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(2, position, shadow, (ar_vector3f_t){ 0.0f,  1.0f,  0.0f}, (ar_vector3f_t){0,  0,  1});
    generate_point_vp(3, position, shadow, (ar_vector3f_t){ 0.0f, -1.0f,  0.0f}, (ar_vector3f_t){0,  0, -1});
    generate_point_vp(4, position, shadow, (ar_vector3f_t){ 0.0f,  0.0f,  1.0f}, (ar_vector3f_t){0, -1,  0});
    generate_point_vp(5, position, shadow, (ar_vector3f_t){ 0.0f,  0.0f, -1.0f}, (ar_vector3f_t){0, -1,  0});
}

void shadows_point_update(shadows_point_t *shadow, ar_vector3f_t position) {
    (void)shadow;
    (void)position;
    // recalculate View/Projection matrices
    //generate_point_vps(shadow, position);
    //shadow->collider.position = position;
    //shadow->camera.camera.position = position;
    //ar_camera_update(&shadow->camera.camera);
}

shadows_point_t shadows_point_init(ar_vector3f_t position, int width, int height, float far_plane) {
    shadows_point_t shadow;
    if (shader_point_shadow == NULL) {
        shader_point_shadow = ar_shader_new("Point Shadow");
        ar_shader_attach(shader_point_shadow, AR_SHADER_VERTEX, "../shaders/shadows/point_vert.glsl");
        ar_shader_attach(shader_point_shadow, AR_SHADER_FRAGMENT, "../shaders/shadows/point_frag.glsl");
        ar_shader_attach(shader_point_shadow, AR_SHADER_GEOMETRY, "../shaders/shadows/point_geom.glsl");
    }
    shadow.shader = shader_point_shadow;
    
    shadow.width = width;
    shadow.height = height;
    
    const float near = 1.0f;
    shadow.far_plane = far_plane;
    // generate perspective matrix
    ar_math_perspective(&shadow.mat_perspective, math_deg_to_rad(90.0f), (float)width/(float)height, near, far_plane);
    
    //ar_framebuffer_cubemap_init(&shadow.cubemap, width, height);
    generate_point_vps(&shadow, position);
    
    //shadow.framebuffer = ar_framebuffer_new(width, height);
    //shadow.framebuffer.texture = shadow.cubemap.map;
    //shadow.framebuffer.texture.bind_type = AR_TEXTURE_CUBEMAP;
    //ar_framebuffer_bind(&shadow.framebuffer);
    
    //ar_framebuffer_texture(&shadow.framebuffer, GL_DEPTH_ATTACHMENT);
    //texture_set_parameter(shadow.framebuffer.texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //shadow.collider = physics_collider_new(PHYSICS_COLLIDER_AABB);
    //shadow.collider.position = position;
    //shadow.collider.dimensions = (ar_vector3f_t){far_plane, far_plane, far_plane};
    
    //shadow.camera = ar_camera_perspective_new();
    //shadow.camera.camera.position = position;
    //ar_camera_update(&shadow.camera.camera);
    
    shadow.shadow_map_id = 0;
    
    return shadow;
}

void shadows_send_uniforms(shadows_point_t *shadow, ar_vector3f_t position) {
    char str[64];
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(str, "shadow_matrices[%d]", i);
        ar_shader_set_uniform(shadow->shader, AR_SHADER_MAT4, str, &shadow->point_vps[i]);
    }
    ar_shader_set_uniform(shadow->shader, AR_SHADER_VEC3F, "light_pos", &position);
}

void shadows_point_render(shadows_point_t *shadow, ar_vector3f_t position, ar_shader_t *shader_main) {
    if (shadow->shader == NULL)
        return;
        
    ar_mat4_t shadow_mat_bias;
    ar_mat4_set(
        &shadow_mat_bias,
        (float []){
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f
        }
    );
    shadow_mat_bias = ar_mat4_mul(shadow_mat_bias, shadow->point_vps[0]);
        
    ar_framebuffer_bind(&shadow->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ar_shader_use(shader_main);
    ar_shader_set_uniform(shader_main, AR_SHADER_MAT4, "shadow_bias", &shadow_mat_bias);
    ar_shader_use(shadow->shader);
    ar_shader_set_uniform(shader_main, AR_SHADER_FLOAT, "far_plane", &shadow->far_plane);
    shadows_send_uniforms(shadow, position);
    
    //ar_objects_draw(shadow->shader, &shadow_cam, false);
    //ar_scene_objects_render(ar_scene_get_selected(), shadow->shader, &shadow->camera.camera, false);
    
    //ar_framebuffer_texture(&shadow->framebuffer, GL_DEPTH_ATTACHMENT);
    ar_shader_use(shader_main);
    
    ar_framebuffer_bind(NULL);
}

/*void shadows_update(light_t *light, int width, int height) {
    //generate_vp(direction, center);
    gen_vps(light, width, height);
}*/

/*void shadows_render(shader_t *shader_main, camera_t *camera) {
    if (shader_depth == NULL)
        return;
    (void)camera;
    render_set_target(RENDER_TARGET_FRAMEBUFFER, &shadow_fb);
    framebuffer_bind(&shadow_fb);
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_use(shader_main);
    
    shader_set_mat4(shader_main, "shadow_bias", &shadow_mat_bias);
    shader_use(shader_depth);
    shadows_send_uniforms();
    //shader_use(shader_main);
    //glCullFace(GL_FRONT);
    //handle_call(HANDLE_RENDER_MESHES, camera);
    objects_draw(shader_depth, &shadow_cam);
    //glCullFace(GL_BACK);
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    shader_use(shader_main);
    framebuffer_bind(NULL);
    render_set_target(RENDER_TARGET_FRAMEBUFFER, NULL);
}*/

void shadows_destroy(shadows_point_t *shadow) {
    if (shadow->shader == NULL)
        return;
    ar_framebuffer_destroy(&shadow->framebuffer);
    if (shader_point_shadow != NULL) {
        ar_shader_destroy(shader_point_shadow);
    }
    shadow->shader = NULL;
}
