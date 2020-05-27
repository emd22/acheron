#include <f3d/engine/rendering/render.h>
#include <f3d/engine/rendering/render_view.h>
#include <f3d/engine/scene/scene.h>
#include <f3d/engine/engine.h>

#include <string.h>
#include <stdlib.h>

int render_all_objects(void *ptr);

shader_t *shader_main;

void init_gl() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_msg(LOG_FATAL, "Could not initialize GLEW\n", 0);
        exit(1);
    }
    
    // enable vsync
    SDL_GL_SetSwapInterval(-1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //glEnable(GL_MULTISAMPLE);

    glClearColor(0.0, 0.0, 0.0, 1);
    glViewport(0, 0, default_window->width, default_window->height);
    
    handles_init();
    meshes_init();
    
    glEnable(GL_FRAMEBUFFER_SRGB);
    
    // fix overlapping polygons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
}

int render_all_objects(void *arg) {
    camera_t *cam = (camera_t *)arg;
    
    //skybox_render(&scenes[0].skybox, cam);
    scene_objects_render(selected_scene, shader_main, cam, true);
    
    return 0;
}

void render_init() {
    init_gl();
    shader_main = shader_new("Main");
    shader_attach(shader_main, SHADER_VERTEX, "../shaders/m_vert.glsl");
    shader_attach(shader_main, SHADER_FRAGMENT, "../shaders/m_frag.glsl");
    
    shader_use(shader_main);
    
    handle_set(HANDLE_RENDER_MESHES, &render_all_objects);
}

void render_init_shadows(scene_t *scene, int width, int height) {
    light_t *point;
    point = light_get(&scene->lights, NULL, LIGHT_POINT);
    if (point == NULL)
        return;
    //shadows_init(point, width, height);
    (void)width;
    (void)height;
}

void render_all() {
    int i;
    for (i = 0; i < scenes_index; i++) {
        scene_render(shader_main, &scenes[i]);
    }
}

void render_destroy(void) {
    int i;
    for (i = 0; i < scenes_index; i++) {
        scene_destroy(&scenes[i]);
    }
    meshes_cleanup();
    textures_cleanup();
    shader_destroy(shader_main);
    //shadows_destroy();
}
