#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/math.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/object/object.h>
#include <f3d/engine/types.h>

#include <stdio.h>

//static mat4_t shadow_mat_vp;
//static mat4_t shadow_mat_bias;
//framebuffer_t shadow_fb;
camera_t shadow_cam;

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

void generate_point_vp(int index, light_t *light, shadows_point_t *shadow, vector3f_t offset, vector3f_t upvec, float aspect) {
    mat4_t perspective;
    const float near = 1.0f;
    const float far  = 50.0f;
    math_perspective(&perspective, math_deg_to_rad(90.0f), aspect, near, far);
    
    vector3f_t to;
    vec3f_add(&to, light->position, offset);
    shadow->point_vps[index] = math_lookat(light->position, to, upvec);
    shadow->point_vps[index] = mat4_mul(perspective, shadow->point_vps[index]);
}

void generate_point_vps(shadows_point_t *shadow, int width, int height) {
    const float aspect = (float)width/(float)height;
    generate_point_vp(0, shadow->light, shadow, (vector3f_t){ 1.0f,  0.0f,  0.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(1, shadow->light, shadow, (vector3f_t){-1.0f,  0.0f,  0.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(2, shadow->light, shadow, (vector3f_t){ 0.0f,  1.0f,  0.0f}, (vector3f_t){0,  0,  1}, aspect);
    generate_point_vp(3, shadow->light, shadow, (vector3f_t){ 0.0f, -1.0f,  0.0f}, (vector3f_t){0,  0, -1}, aspect);
    generate_point_vp(4, shadow->light, shadow, (vector3f_t){ 0.0f,  0.0f,  1.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(5, shadow->light, shadow, (vector3f_t){ 0.0f,  0.0f, -1.0f}, (vector3f_t){0, -1,  0}, aspect);
}

shadows_point_t shadows_point_init(light_t *light, int width, int height) {
    shadows_point_t shadow;
    shadow.shader = shader_new("Point Shadow");
    shader_attach(shadow.shader, SHADER_VERTEX, "../shaders/shadows/point_vert.glsl");
    shader_attach(shadow.shader, SHADER_FRAGMENT, "../shaders/shadows/point_frag.glsl");
    shader_attach(shadow.shader, SHADER_GEOMETRY, "../shaders/shadows/point_geom.glsl");
    shader_link(shadow.shader);
    
    shadow.width = width;
    shadow.height = height;
    
    shadow.light = light;
    framebuffer_cubemap_init(&shadow.cubemap, width, height);
    generate_point_vps(&shadow, width, height);
    
    shadow.framebuffer = framebuffer_new(width, height, GL_DEPTH_ATTACHMENT, false);
    shadow.framebuffer.texture = shadow.cubemap.map;
    shadow.framebuffer.texture_target = GL_TEXTURE_CUBE_MAP;
    framebuffer_bind(&shadow.framebuffer);
    
    framebuffer_texture(&shadow.framebuffer, GL_DEPTH_ATTACHMENT);
    return shadow;
}

void shadows_send_uniforms(shadows_point_t *shadow) {
    char str[64];
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(str, "shadow_matrices[%d]", i);
        shader_set_mat4(shadow->shader, str, &shadow->point_vps[i]);
    }
    shader_set_vec3f(shadow->shader, "light_pos", shadow->light->position);
}

void shadows_point_render(shadows_point_t *shadow, shader_t *shader_main) {
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
        
    framebuffer_bind(&shadow->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_use(shader_main);
    shader_set_mat4(shader_main, "shadow_bias", &shadow_mat_bias);
    shader_use(shadow->shader);
    shadows_send_uniforms(shadow);
    objects_draw(shadow->shader, &shadow_cam);
    framebuffer_texture(&shadow->framebuffer, GL_DEPTH_ATTACHMENT);
    shader_use(shader_main);
    
    framebuffer_bind(NULL);
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
    framebuffer_destroy(&shadow->framebuffer);
    shader_destroy(shadow->shader);
}
