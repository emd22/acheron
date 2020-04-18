#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/math.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

framebuffer_t shadow_fb;
static camera_t shadow_cam;
static mat4_t shadow_mat_vp;
static mat4_t shadow_mat_bias;

void generate_vp(vector3f_t direction, vector3f_t center) {
    const vector3f_t up = (vector3f_t){0, 1, 0};
    shadow_cam.mat_view = math_lookat(direction, center, up);
    shadow_mat_vp = mat4_mul(shadow_cam.mat_projection, shadow_cam.mat_view);
}

void shadows_init(int width, int height, vector3f_t direction, vector3f_t center) {
    shadow_fb = framebuffer_new(width, height, 16, GL_DEPTH_ATTACHMENT);
    framebuffer_bind(&shadow_fb);
    texture_t *tex = shadow_fb.texture;
    tex->draw_type = TEXTURE_TYPE_DEPTH;
    tex->data_type = TEXTURE_TYPE_DEPTH;
    // make a 16 bit texture with using depth parameters
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // projection
    const float clip_near = -10;
    const float clip_far  = 200;
    math_ortho(&shadow_cam.mat_projection, -10, 10, -10, 10, clip_near, clip_far);
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
    framebuffer_bind(&shadow_fb);

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
    framebuffer_bind(NULL);
}

void shadows_destroy() {
    framebuffer_destroy(&shadow_fb);
    
}
