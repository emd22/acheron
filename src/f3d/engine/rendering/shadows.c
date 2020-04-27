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

static mat4_t shadow_mat_vp;
static mat4_t shadow_mat_bias;
framebuffer_t shadow_fb;
camera_t shadow_cam;
static light_t *shadow_light;

static mat4_t shadow_point_vps[6];

cubemap_t shadow_cubemap;

shader_t *shader_depth = NULL;

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

void generate_point_vp(int index, light_t *light, vector3f_t offset, vector3f_t upvec, float aspect) {
    mat4_t perspective;
    const float near = 1.0f;
    const float far  = 50.0f;
    math_perspective(&perspective, math_deg_to_rad(90.0f), aspect, near, far);
    
    vector3f_t to;
    vec3f_add(&to, light->position, offset);
    shadow_point_vps[index] = math_lookat(light->position, to, upvec);
    shadow_point_vps[index] = mat4_mul(perspective, shadow_point_vps[index]);
}

void gen_vps(light_t *light, int width, int height) {
    const float aspect = (float)width/(float)height;
    generate_point_vp(0, light, (vector3f_t){ 1.0f,  0.0f,  0.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(1, light, (vector3f_t){-1.0f,  0.0f,  0.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(2, light, (vector3f_t){ 0.0f,  1.0f,  0.0f}, (vector3f_t){0,  0,  1}, aspect);
    generate_point_vp(3, light, (vector3f_t){ 0.0f, -1.0f,  0.0f}, (vector3f_t){0,  0, -1}, aspect);
    generate_point_vp(4, light, (vector3f_t){ 0.0f,  0.0f,  1.0f}, (vector3f_t){0, -1,  0}, aspect);
    generate_point_vp(5, light, (vector3f_t){ 0.0f,  0.0f, -1.0f}, (vector3f_t){0, -1,  0}, aspect);
}

void shadows_init(light_t *light, int width, int height) {
    if (shader_depth == NULL) {
        shader_depth = shader_new("Depth");
        shader_attach(shader_depth, SHADER_VERTEX, "../shaders/depth_vert.glsl");
        shader_attach(shader_depth, SHADER_FRAGMENT, "../shaders/depth_frag.glsl");
        shader_attach(shader_depth, SHADER_GEOMETRY, "../shaders/depth_geom.glsl");
        shader_link(shader_depth);
    }
    
    shadow_light = light;
    
    framebuffer_cubemap_init(&shadow_cubemap, width, height);

    shadow_fb = framebuffer_new(width, height, GL_DEPTH_ATTACHMENT, false);
    shadow_fb.texture = shadow_cubemap.map;
    shadow_fb.texture_target = GL_TEXTURE_CUBE_MAP;
    framebuffer_bind(&shadow_fb);
    
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    // make a 16 bit texture with using depth parameters
    //framebuffer_generate_texture(&shadow_fb, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    
    // projection
    //generate_vp(direction, center);
    gen_vps(light, width, height);
    
    mat4_set(
        &shadow_mat_bias,
        (float []){
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f
        }
    );
    shadow_mat_bias = mat4_mul(shadow_mat_bias, shadow_mat_vp);
    framebuffer_bind(NULL);
}

void shadows_update(light_t *light, int width, int height) {
    //generate_vp(direction, center);
    gen_vps(light, width, height);
}

void shadows_send_uniforms() {
    char str[64];
    int i;
    for (i = 0; i < 6; i++) {
        sprintf(str, "shadow_matrices[%d]", i);
        shader_set_mat4(shader_depth, str, &shadow_point_vps[i]);
    }
    shader_set_vec3f(shader_depth, "light_pos", shadow_light->position);
}

void shadows_render(shader_t *shader_main, camera_t *camera) {
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
}

void shadows_destroy() {
    if (shader_depth == NULL)
        return;
    framebuffer_destroy(&shadow_fb);
    shader_destroy(shader_depth);
}
