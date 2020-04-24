#include <f3d/engine/rendering/scene.h>

#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/core/handles.h>
#include <string.h>

scene_t scenes[8];
int scenes_index = 0;

scene_t *scene_new(const char *name) {
    scene_t *scene = &scenes[scenes_index++];
    strcpy(scene->name, name);
    memset(&scene->views[0], 0, sizeof(render_view_t));
    scene->views_index = 1;
    scene->lights_index = 0;
    scene->flags = 0;
    /*texture_t *textures[] = {
        texture_load_data(NULL, "../images/skybox/right.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/left.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/down.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/up.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/front.bmp", IMAGE_BMP),
        texture_load_data(NULL, "../images/skybox/back.bmp", IMAGE_BMP)
    };
    scene->skybox = skybox_new(textures);*/
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

void scene_render(shader_t *shader_main, scene_t *scene) {
    int view_count = 0;
    
    if (scene != NULL)
        view_count = scene->views_index;
    
    // draw main screen
    // set framebuffer to our 'default' framebuffer
    framebuffer_bind(NULL);
    
    if (shader_depth != NULL) {
        // if shadows are setup, set shadow map in main shader
        shadows_render(shader_main);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, shadow_fb.texture->id);
        shader_set_int(shader_main, "shadow_map", 4);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_call(HANDLE_RENDER_MESHES, selected_camera);
    shader_use(shader_main);
    
    // draw addtional views    
    int i;
    render_view_t *view;
    for (i = 1; i < view_count; i++) {
        view = &scene->views[i];
        
        framebuffer_bind(&view->framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handle_call(HANDLE_RENDER_MESHES, view->camera);
        
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

void scene_attach(scene_t *scene, int type, void *ptr) {
    if (type == SCENE_SKYBOX) {
        memcpy(&scene->skybox, ptr, sizeof(skybox_t));
        scene->flags |= SCENE_ENABLE_SKYBOX;
        //skybox_new(&scene->skybox.cubemap);
    }
    else if (type == SCENE_LIGHT) {
        scene->lights[scene->lights_index++] = (light_t *)ptr;
    }
}

void scene_destroy(scene_t *scene) {
    int i;
    for (i = 1; i < scene->views_index; i++) {
        framebuffer_destroy(&scene->views[i].framebuffer);
    }
}
