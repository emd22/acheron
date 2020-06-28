#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/camera.h>

#include <f3d/engine/core/handles.h>
#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/core/log.h>

#include <f3d/engine/scene/sc_scene.h>
#include <f3d/engine/scene/sc_object.h>
#include <f3d/engine/types.h>

#include <stdio.h>

//static mat4_t shadow_mat_vp;
//static mat4_t shadow_mat_bias;
//framebuffer_t shadow_fb;
camera_t shadow_cam;
static ar_shader_t *shader_point_shadow = NULL;

/*
void generate_vp(vector3f_t direction, vector3f_t center) {
    const vector3f_t up = (vector3f_t){0, 1, 0};
    const float clip_near = -10;
    const float clip_far  = 20;
    math_ortho(&shadow_cam.mat_projection, -10, 10, -10, 10, clip_near, clip_far);
    mat4_t model_mat;
    shadow_cam.mat_view = math_lookat(direction, center, up);
    mat4_set(&model_mat, MAT4_IDENTITY);
    shadow_mat_vp = mat4_mul(shadow_cam.mat_projection, shadow_cam.mat_view);
    shadow_mat_vp = mat4_mul(shadow_mat_vp, model_mat);
}*/

void generate_point_vp(int index, ar_vector3f_t position, shadows_point_t *shadow, ar_vector3f_t offset, ar_vector3f_t upvec) {
    ar_vector3f_t to;
    ar_vector_add(AR_VEC3F, &position, &offset, &to);
    shadow->point_vps[index] = math_lookat(position, to, upvec);
    shadow->point_vps[index] = mat4_mul(shadow->mat_perspective, shadow->point_vps[index]);
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
    // recalculate View/Projection matrices
    generate_point_vps(shadow, position);
    shadow->collider.position = position;
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
    math_perspective(&shadow.mat_perspective, math_deg_to_rad(90.0f), (float)width/(float)height, near, far_plane);
    
    ar_framebuffer_cubemap_init(&shadow.cubemap, width, height);
    generate_point_vps(&shadow, position);
    
    shadow.framebuffer = ar_framebuffer_new(width, height, GL_DEPTH_ATTACHMENT, false);
    shadow.framebuffer.texture = shadow.cubemap.map;
    shadow.framebuffer.texture_target = GL_TEXTURE_CUBE_MAP;
    ar_framebuffer_bind(&shadow.framebuffer);
    
    ar_framebuffer_texture(&shadow.framebuffer, GL_DEPTH_ATTACHMENT);
    //texture_set_parameter(shadow.framebuffer.texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //shadow.collider = physics_collider_new(PHYSICS_COLLIDER_AABB);
    //shadow.collider.position = position;
    //shadow.collider.dimensions = (ar_vector3f_t){far_plane, far_plane, far_plane};
    
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
        
    mat4_t shadow_mat_bias;
    mat4_set(
        &shadow_mat_bias,
        (float []){
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f
        }
    );
    shadow_mat_bias = mat4_mul(shadow_mat_bias, shadow->point_vps[0]);
        
    ar_framebuffer_bind(&shadow->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ar_shader_use(shader_main);
    ar_shader_set_uniform(shader_main, AR_SHADER_MAT4, "shadow_bias", &shadow_mat_bias);
    ar_shader_use(shadow->shader);
    ar_shader_set_uniform(shader_main, AR_SHADER_FLOAT, "far_plane", &shadow->far_plane);
    shadows_send_uniforms(shadow, position);
    
    //objects_draw(shadow->shader, &shadow_cam, false);
    ar_scene_objects_render(ar_scene_get_selected(), shadow->shader, &shadow_cam, false);
    
    ar_framebuffer_texture(&shadow->framebuffer, GL_DEPTH_ATTACHMENT);
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
    ar_shader_destroy(shadow->shader);
}
