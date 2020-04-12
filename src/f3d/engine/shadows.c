#include <f3d/engine/shadows.h>
#include <f3d/engine/framebuffer.h>
#include <f3d/engine/camera.h>
#include <f3d/engine/math.h>
#include <f3d/engine/log.h>
#include <f3d/engine/types.h>

framebuffer_t shadow_fb;
static camera_t shadow_cam;
static mat4_t shadow_mat_vp;
static mat4_t shadow_mat_bias;

void shadows_init(int width, int height, light_t *light) {
    shadow_fb = framebuffer_new(width, height, 16);
    framebuffer_bind(&shadow_fb);
    texture_t *tex = shadow_fb.texture;
    tex->draw_type = TEXTURE_TYPE_DEPTH16;
    tex->data_type = TEXTURE_TYPE_DEPTH;
    // make a 16 bit texture with using depth parameters
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // projection
    const float clip_near = -10;
    const float clip_far  = 20;
    math_ortho(&shadow_cam.mat_projection, -10, 10, -10, 10, clip_near, clip_far);
    
    const vector3f_t up = (vector3f_t){0, 1, 0};
    const vector3f_t center = (vector3f_t){0, 0, 0};
    shadow_cam.mat_view = math_lookat(light->direction, center, up);
    shadow_mat_vp = mat4_mul(shadow_cam.mat_projection, shadow_cam.mat_view);
    
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

void shadows_render(shader_t *shader_main, shader_t *shader_depth, void (*render_scene)(shader_t *, camera_t *)) {
    framebuffer_bind(&shadow_fb);

    shader_set_mat4(shader_main, "shadow_bias", &shadow_mat_bias);
    shader_use(shader_depth);
    shader_set_mat4(shader_depth, "depth_mvp", &shadow_mat_bias);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    shader_use(shader_main);
    render_scene(shader_main, &shadow_cam);
    //log_msg(LOG_INFO, "err\n", 0);
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    shader_use(shader_main);
    framebuffer_bind(NULL);
}

void shadows_destroy() {
    framebuffer_destroy(&shadow_fb);
    
}
