#include <f3d/engine/rendering/render.h>
#include <f3d/engine/rendering/render_view.h>
#include <f3d/engine/scene/sc_scene.h>
#include <f3d/engine/engine.h>
#include <f3d/engine/acheron.h>

#include <string.h>
#include <stdlib.h>

int render_all_objects(void *ptr);

ar_shader_t *shader_main;

void init_gl() {    
    handles_init();
    meshes_init();
    
    glEnable(GL_FRAMEBUFFER_SRGB);
    
    // fix overlapping polygons
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    
    //glEnable(GL_CULL_FACE);
}

int render_all_objects(void *arg) {
    camera_t *cam = (camera_t *)arg;
    
    //skybox_render(&scenes[0].skybox, cam);
    ar_scene_objects_render(ar_scene_get_selected(), shader_main, cam, true);
    
    return 0;
}

void render_init() {
    init_gl();
    shader_main = ar_shader_new("Main");
    ar_shader_attach(shader_main, AR_SHADER_VERTEX, "../shaders/m_vert.glsl");
    ar_shader_attach(shader_main, AR_SHADER_FRAGMENT, "../shaders/m_frag.glsl");
    
    ar_shader_use(shader_main);
    
    handle_set(HANDLE_RENDER_MESHES, &render_all_objects);
}

void render_init_shadows(ar_scene_t *scene, int width, int height) {
    ar_light_t *point;
    point = ar_light_get(&scene->lights, NULL, AR_LIGHT_POINT);
    if (point == NULL)
        return;
    //shadows_init(point, width, height);
    (void)width;
    (void)height;
}

void render_all() {
    unsigned i;
    for (i = 0; i < scenes.index; i++) {
        ar_scene_render(shader_main, (ar_scene_t *)ar_buffer_get(&scenes, i));
    }
}

void render_destroy(void) {
    meshes_cleanup();
    textures_cleanup();
    ar_shader_destroy(shader_main);
}
