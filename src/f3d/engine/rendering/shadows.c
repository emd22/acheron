#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/math.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/object/object.h>
#include <f3d/engine/types.h>

framebuffer_t shadow_fb;
camera_t shadow_cam;
static mat4_t shadow_mat_vp;
static mat4_t shadow_mat_bias;

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
}

void shadows_init(int width, int height, vector3f_t direction, vector3f_t center) {
    shadow_fb = framebuffer_new(width, height, GL_DEPTH_ATTACHMENT);
    framebuffer_bind(&shadow_fb);
    // make a 16 bit texture with using depth parameters
    framebuffer_generate_texture(&shadow_fb, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
    
    // projection
    generate_vp(direction, center);
    
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

void shadows_update(vector3f_t direction, vector3f_t center) {
    generate_vp(direction, center);
}

void shadows_render(shader_t *shader_main, shader_t *shader_depth) {
    render_set_target(RENDER_TARGET_FRAMEBUFFER, &shadow_fb);
    shader_set_mat4(shader_main, "shadow_bias", &shadow_mat_bias);
    shader_use(shader_depth);
    shader_set_mat4(shader_depth, "depth_mvp", &shadow_mat_vp);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    shader_use(shader_main);
    //glCullFace(GL_FRONT);
    handle_call(HANDLE_RENDER_MESHES, &shadow_cam);
    //glCullFace(GL_BACK);
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    shader_use(shader_main);
    render_set_target(RENDER_TARGET_FRAMEBUFFER, NULL);
}

void shadows_destroy() {
    framebuffer_destroy(&shadow_fb);
    
}
