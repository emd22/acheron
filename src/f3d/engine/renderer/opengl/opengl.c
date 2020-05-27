#include <f3d/engine/renderer/opengl/opengl.h>

typedef enum {
    SB_RENDERER_BLEND,
    SB_RENDERER_CULL_FACE,
    SB_RENDERER_DEPTH_TEST,
    SB_RENDERER_FRAMEBUFFER_SRGB,
    SB_RENDERER_MULTISAMPLE,
    SB_RENDERER_LINE_SMOOTH,
    SB_RENDERER_POLYGON_SMOOTH,
} sb_renderer_enable_t;



static int get_option_code_gl(sb_render_enable_t code) {
    switch (code) {
        case SB_RENDERER_BLEND:
            return GL_BLEND;
            
        case SB_RENDERER_CULL_FACE:
            return GL_CULL_FACE;
            
        case SB_RENDERER_DEPTH_TEST:
            return GL_DEPTH_TEST;
            
        case SB_RENDERER_FRAMEBUFFER_SRGB:
            return GL_FRAMEBUFFER_SRGB;
            
        case SB_RENDERER_MULTISAMPLE:
            return GL_MULTISAMPLE;
            
        case SB_RENDERER_LINE_SMOOTH:
            return GL_LINE_SMOOTH;

        case SB_RENDERER_POLYGON_SMOOTH:
            return GL_POLYGON_SMOOTH;
            
        default:
            return -1;
    }
}

void sb_renderer_enable(sb_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
    if (glcode != -1)
        glEnable(glcode);
}

void sb_renderer_disable(sb_renderer_enable_t code) {
    int glcode = get_option_code_gl(code);
    if (glcode != -1)
        glDisable(glcode);
}

static void opengl_renderer_init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, default_window.width, default_window.height);
    sb_renderer_enable(SB_RENDERER_FRAMEBUFFER_SRGB);
    sb_renderer_enable(SB_RENDERER_DEPTH_TEST);
    sb_renderer_enable(SB_RENDERER_CULL_FACE);
    glDepthFunc(GL_LESS);
}
