#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/math.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/object/object.h>
#include <f3d/engine/types.h>

static mat4_t shadow_mat_vp;
static mat4_t shadow_mat_bias;
framebuffer_t shadow_fb;
camera_t shadow_cam;

static mat4_t shadow_point_vps[6];

static cubemap_t shadow_cubemap;

shader_t *shader_depth = NULL;

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

void generate_point_vps(light_t *light, vector3f_t offset, vector3f_t upvec, float aspect) {
    mat4_t perspective;
    const float near = 1.0f;
    const float far  = 25.0f;
    math_perspective(&perspective, math_deg_to_rad(90.0f), aspect, near, far);
    
    vector3f_t to;
    int i;
    for (i = 0; i < 6; i++) {
        vec3f_add(&to, light->position, offset);
        shadow_point_vps[i] = math_lookat(light->position, to, upvec);
        shadow_point_vps[i] = mat4_mul(perspective, shadow_point_vps[i]);
    }
}

void shadows_init(int width, int height, vector3f_t direction, vector3f_t center) {
    if (shader_depth == NULL) {
        shader_depth = shader_new("Depth");
        shader_attach(shader_depth, SHADER_VERTEX, "../shaders/depth_vert.glsl");
        shader_attach(shader_depth, SHADER_FRAGMENT, "../shaders/depth_frag.glsl");
        shader_link(shader_depth);
    }
    
    framebuffer_cubemap_init(&shadow_cubemap, width, height);

    shadow_fb = framebuffer_new(width, height, GL_DEPTH_ATTACHMENT, false);
    shadow_fb.texture = shadow_cubemap.map;
    shadow_fb.texture_target = GL_TEXTURE_CUBE_MAP;
    framebuffer_bind(&shadow_fb);
    
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    // make a 16 bit texture with using depth parameters
    //framebuffer_generate_texture(&shadow_fb, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
    
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

void shadows_render(shader_t *shader_main) {
    if (shader_depth == NULL)
        return;
    //render_set_target(RENDER_TARGET_FRAMEBUFFER, &shadow_fb);
    framebuffer_bind(&shadow_fb);
    shader_set_mat4(shader_main, "shadow_bias", &shadow_mat_bias);
    shader_use(shader_depth);
    shader_set_mat4(shader_depth, "depth_mvp", &shadow_mat_vp);
    //glClear(GL_DEPTH_BUFFER_BIT);
    shader_use(shader_main);
    //glCullFace(GL_FRONT);
    handle_call(HANDLE_RENDER_MESHES, &shadow_cam);
    //glCullFace(GL_BACK);
    framebuffer_texture(&shadow_fb, GL_DEPTH_ATTACHMENT);
    shader_use(shader_main);
    framebuffer_bind(NULL);
    //render_set_target(RENDER_TARGET_FRAMEBUFFER, NULL);
}

void shadows_destroy() {
    if (shader_depth == NULL)
        return;
    framebuffer_destroy(&shadow_fb);
    shader_destroy(shader_depth);
}
