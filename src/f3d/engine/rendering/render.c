#include <f3d/engine/rendering/render.h>
#include <f3d/engine/engine.h>

#include <string.h>

int render_all_objects(void *ptr);

static scene_t scenes[8];
static int scenes_index = 0;
shader_t *shader_main, *shader_depth;

scene_t *selected_scene = NULL;

scene_t *scene_new(const char *name) {
    scene_t *scene = &scenes[scenes_index++];
    strcpy(scene->name, name);
    memset(&scene->views[0], 0, sizeof(render_view_t));
    scene->views_index = 1;
    scene->flags = 0;
    texture_t *textures[] = {
        texture_load_data(NULL, "../images/skybox/right.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/left.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/down.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/up.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/front.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/back.bmp", IMAGE_BMP)
    };
    scene->skybox = skybox_new(textures);
    return scene;
}

render_view_t *scene_new_view(scene_t *scene, camera_t *camera, int width, int height, int attachment) {
    render_view_t *view = &scene->views[scene->views_index++];
    view->framebuffer = framebuffer_new(width, height, attachment, true);
    framebuffer_generate_texture(&view->framebuffer, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    view->src_dimensions = (vector2f_t){width, height};
    view->dest_dimensions = (vector2f_t){width, height};
    view->camera = camera;
    return view;
}

void scene_attach(scene_t *scene, int type, void *ptr) {
    if (type == SCENE_SKYBOX) {
        memcpy(&scene->skybox, ptr, sizeof(skybox_t));
        scene->flags |= SCENE_ENABLE_SKYBOX;
        //skybox_new(&scene->skybox.cubemap);
    }
}

void render_init() {
    //shader_main = shader_load();
    shader_main = shader_new("Main");
    shader_attach(shader_main, SHADER_VERTEX, "../shaders/m_vert.glsl");
    shader_attach(shader_main, SHADER_FRAGMENT, "../shaders/m_frag.glsl");
    shader_link(shader_main);
    
    shader_depth = shader_new("Depth");
    shader_attach(shader_depth, SHADER_VERTEX, "../shaders/depth_vert.glsl");
    shader_attach(shader_depth, SHADER_FRAGMENT, "../shaders/depth_frag.glsl");
    shader_link(shader_depth);
    handle_set(HANDLE_RENDER_MESHES, &render_all_objects);
}

int render_all_objects(void *arg) {
    camera_t *cam = (camera_t *)arg;
    
    skybox_render(&scenes[0].skybox, cam);
    shader_use(shader_main);
    objects_draw(shader_main, cam);
    
    return 0;
}

void render_scene(scene_t *scene) {
    int view_count = 0;
    
    if (scene != NULL)
        view_count = scene->views_index;
    // draw main screen
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    shadows_render(shader_main, shader_depth);
    //framebuffer_bind(NULL);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadow_fb.texture->id);
    shader_set_int(shader_main, "shadow_map", 4);
    render_all_objects(selected_camera);
    shader_use(shader_main);
    
    // draw addtional views    
    int i;
    render_view_t *view;
    for (i = 1; i < view_count; i++) {
        view = &scene->views[i];
        
        framebuffer_bind(&view->framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_all_objects(view->camera);
        
        glBindFramebuffer(GL_READ_FRAMEBUFFER, view->framebuffer.fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        
        glBlitFramebuffer(
            // source framebuffer
            view->src_position.x, view->src_position.y, // position
            view->src_dimensions.x+view->src_position.x, // width
            view->src_dimensions.y+view->src_position.y, // height
            // destination framebuffer (default fb)
            view->dest_position.x, view->dest_position.y,  // positions
            view->dest_dimensions.x+view->dest_position.x, // width
            view->dest_dimensions.y+view->dest_position.y, // height
            GL_COLOR_BUFFER_BIT, GL_NEAREST
        );
    }

}

void render_all() {
    int i;
    for (i = 0; i < scenes_index; i++) {
        render_scene(&scenes[i]);
    }
}
