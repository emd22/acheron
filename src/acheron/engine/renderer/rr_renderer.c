#include <acheron/engine/renderer/rr_renderer.h>
#include <acheron/engine/acheron.h>

#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/types.h>

#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/renderer/rr_shaderman.h>
#include <acheron/engine/scene/sc_scene.h>

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

ar_renderer_instance_t ar_renderer_init() {
    ar_renderer_instance_t instance;
    ar_renderer_intern_init(&instance);
    
    // TODO: replace with renderer independent functions
    SDL_GL_SetSwapInterval(1);
    
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);

    // load default render shader, set as renderer shader
    ar_shader_t *render_shader = ar_shader_new("Render Shader");
    ar_shader_attach(render_shader, AR_SHADER_VERTEX, "../shaders/render_vert.glsl");
    ar_shader_attach(render_shader, AR_SHADER_FRAGMENT, "../shaders/render_frag.glsl");
    ar_shader_use(render_shader);
    ar_shaderman_set_render_shader(render_shader);
    
    ar_handle_set(AR_HANDLE_RENDER_MESHES, &ar_render_scene_objects);

    return instance;
}

int ar_render_scene_objects(void *camera) {
    ar_camera_t *cam = (ar_camera_t *)camera;
    ar_shader_t *shader = ar_shaderman_get_render_shader();
    ar_scene_objects_render(ar_scene_get_selected(), shader, cam, true);
    return 0;
}

void ar_renderer_draw(ar_camera_t *camera) {
    // bind to our draw buffer
    //ar_framebuffer_bind(NULL);
    const ar_shader_t *shader = ar_shaderman_get_render_shader();
    ar_scene_render((ar_shader_t *)shader, ar_scene_get_selected(), camera);
}


void ar_renderer_destroy(ar_renderer_instance_t *instance) {
    instance->renderer_type = AR_RENDERER_NONE;
    SDL_Quit();
}
