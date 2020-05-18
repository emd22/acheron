#include <f3d/engine/rendering/scene.h>

#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/ui.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/time.h>
#include <f3d/engine/limits.h>
#include <string.h>
#include <stdio.h>

scene_t scenes[8];
int scenes_index = 0;
scene_t *selected_scene = NULL;

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

void scene_render_shadows(scene_t *scene, shader_t *shader_main) {
    int i;
    light_t *light;
    for (i = 0; i < scene->lights_index; i++) {
        light = scene->lights[i];
        // if shadows are setup, set shadow map in main shader
        //shadows_point_render(&scene->shadow, shader_main);
        if (light->use_shadows) {
            light_shadow_render(light, shader_main);   
        }
    }
}

void scene_select(scene_t *scene, shader_t *shader_main) {
    selected_scene = scene;
    int i;
    for (i = 0; i < scene->lights_index; i++) {
        light_init(scene->lights[i], shader_main);
        light_update(scene->lights[i], shader_main);
    }
}

light_t *get_nearest_light(scene_t *scene, object_t *object, shader_t *shader_main) {
    float obj_dist = (object->position.x+object->position.y+object->position.z)/3.0f;
    float light_dist = 0;
    float dist;
    int i;
    light_t *light;
    for (i = 0; i < scene->lights_index; i++) {
        light = scene->lights[i];
        light_dist = (light->position.x+light->position.y+light->position.z)/3.0f;
        dist = +(obj_dist-light_dist);
        if (dist <= light->radius) {
            light_shadow_render(light, shader_main);
            return light;
        }
    }
    return NULL;
}

void scene_object_update(scene_t *scene, render_object_t *object, shader_t *shader_main) {
    get_nearest_light(scene, object, shader_main);
}

void scene_render(shader_t *shader_main, scene_t *scene) {
    int view_count = 0;
    
    if (scene != NULL)
        view_count = scene->views_index;
    
    // draw main screen
    // set framebuffer to our 'default' framebuffer
    framebuffer_bind(NULL);
    
    //if (shader_depth != NULL) {
    int i;
    light_t *light;
    char str[48];
    for (i = 0; i < MAX_LIGHTS; i++) {
        if (i >= scene->lights_index) {
            sprintf(str, "pointLights[%d].shadow_map", i);
            shader_set_int(shader_main, str, 4);
            continue;
        }
        light = scene->lights[i];
        // if shadows are setup, set shadow map in main shader
        //shadows_point_render(&scene->shadow, shader_main);
        if (light->use_shadows) {
            glActiveTexture(GL_TEXTURE4+light->point_shadow.shadow_map_id);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, light->point_shadow.framebuffer.texture->id);
            texture_bind(light->point_shadow.framebuffer.texture);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_call(HANDLE_RENDER_MESHES, selected_camera);
    shader_use(shader_main);
    ui_render();
    //shader_use(shader_main);    
    // draw addtional views
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
        if (scene->lights_index == MAX_LIGHTS)
            return;
        scene->lights[scene->lights_index++] = (light_t *)ptr;
    }
}

void scene_destroy(scene_t *scene) {
    int i;
    for (i = 1; i < scene->views_index; i++) {
        framebuffer_destroy(&scene->views[i].framebuffer);
    }
}
