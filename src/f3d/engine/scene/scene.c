#include <f3d/engine/scene/scene.h>

#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/ui.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/time.h>
#include <f3d/engine/limits.h>
#include <string.h>
#include <stdio.h>

#define SCENE_OBJECTS_START_SIZE 64

scene_t scenes[MAX_SCENES];
int scenes_index = 0;
scene_t *selected_scene = NULL;

scene_t *scene_new(const char *name) {
    scene_t *scene = &scenes[scenes_index++];
    strcpy(scene->name, name);
    buffer_init(&scene->lights, BUFFER_STATIC, sizeof(light_t), MAX_SCENE_LIGHTS);
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
    
    scene->objects = object_buffer_new(BUFFER_DYNAMIC, SCENE_OBJECTS_START_SIZE);
    
    return scene;
}

void scene_render_shadows(scene_t *scene, shader_t *shader_main) {
    unsigned i;
    light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = buffer_get(&scene->lights, i);
        // if shadows are setup, set shadow map in main shader
        if (light->use_shadows) {
            light_shadow_render(light, shader_main);   
        }
    }
}

void scene_select(scene_t *scene, shader_t *shader_main) {
    selected_scene = scene;
    unsigned i;
    light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = buffer_get(&scene->lights, i);
        light_init(light, shader_main);
        light_update(light, shader_main);
    }
}

light_t *get_nearest_light(scene_t *scene, object_t *object) {
    float rx, rz;
    unsigned i;
    light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = buffer_get(&scene->lights, i);
        rx = object->position.x-light->position.x;
        rz = object->position.z-light->position.z;
        float radius = light->radius/2;
        if ((rx >= -radius && rx <= radius) &&
            (rz >= -radius && rz <= radius)) {
            return light;
        }
    }
    return NULL;
}

void scene_object_update(scene_t *scene, object_t *object, shader_t *shader_main) {
    light_t *light;
    if ((light = get_nearest_light(scene, object)) != NULL) {
        light_shadow_render(light, shader_main);
    }
}

void scene_objects_render(scene_t *scene, shader_t *shader, camera_t *camera, bool render_materials) {
    objects_draw(&scene->objects, shader, camera, render_materials);
}

void scene_render(shader_t *shader_main, scene_t *scene) {
    //int view_count = 0;
    
    //if (scene != NULL)
    //    view_count = scene->views_index;
    
    // draw main screen
    // set framebuffer to our 'default' framebuffer
    framebuffer_bind(NULL);
    
    //if (shader_depth != NULL) {
    unsigned i;
    light_t *light;
    char str[48];
    for (i = 0; i < MAX_LIGHTS; i++) {
        if (i >= scene->lights.index) {
            sprintf(str, "pointLights[%d].shadow_map", i);
            shader_set_int(shader_main, str, 4);
            continue;
        }
        light = buffer_get(&scene->lights, i);
        // if shadows are setup, set shadow map in main shader
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
}

void *scene_attach(scene_t *scene, scene_attach_type_t type, void *ptr) {
    void *scene_object = NULL;
    if (type == SCENE_SKYBOX) {
        memcpy(&scene->skybox, ptr, sizeof(skybox_t));
        scene->flags |= SCENE_ENABLE_SKYBOX;
        // TODO: add back
        //skybox_new(&scene->skybox.cubemap);
    }
    else if (type == SCENE_LIGHT) {
        if (scene->lights.index == MAX_LIGHTS)
            return NULL;
        scene_object = buffer_push(&scene->lights, ptr);
    }
    else if (type == SCENE_OBJECT) {
        scene_object = buffer_push(&scene->objects.buffer, ptr);
    }
    return scene_object;
}

void scene_destroy(scene_t *scene) {
    (void)scene;
    //int i;
    //for (i = 1; i < scene->views_index; i++) {
        //framebuffer_destroy(&scene->views[i].framebuffer);
    //}
}
