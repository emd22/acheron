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

ar_buffer_t scenes;
static ar_scene_t *selected_scene = NULL;

ar_scene_t *ar_scene_new(const char *name) {
    if (scenes.initialized == false) {
        ar_buffer_init(&scenes, AR_BUFFER_DYNAMIC, sizeof(ar_scene_t), 4);
        ar_log(AR_LOG_INFO, "Init\n", 0);
    }
    
    ar_scene_t *scene = ar_buffer_new_item(&scenes);
    strcpy(scene->name, name);
    ar_buffer_init(&scene->lights, AR_BUFFER_STATIC, sizeof(ar_light_t), MAX_SCENE_LIGHTS);
    scene->flags = 0;
    
    ar_buffer_init(&scene->objects, AR_BUFFER_DYNAMIC, sizeof(ar_object_t *), SCENE_OBJECTS_START_SIZE);
    
    return scene;
}

void ar_scene_render_shadows(ar_scene_t *scene, ar_shader_t *shader_main) {
    unsigned i;
    ar_light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = ar_buffer_get(&scene->lights, i);
        // if shadows are setup, set shadow map in main shader
        if (light->use_shadows) {
            ar_light_shadow_render(light, shader_main);   
        }
    }
}

void ar_scene_select(ar_scene_t *scene, ar_shader_t *shader_main) {
    selected_scene = scene;
    unsigned i;
    ar_light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = ar_buffer_get(&scene->lights, i);
        ar_light_init(light, shader_main);
        ar_light_update(light, shader_main);
    }
}

ar_scene_t *ar_scene_get_selected(void) {
    return selected_scene;
}

ar_light_t *get_nearest_light(ar_scene_t *scene, ar_object_t *object) {
    float rx, rz;
    unsigned i;
    ar_light_t *light;
    for (i = 0; i < scene->lights.index; i++) {
        light = ar_buffer_get(&scene->lights, i);
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

void ar_scene_object_update(ar_scene_t *scene, ar_object_t *object, ar_shader_t *shader_main) {
    ar_light_t *light;
    if ((light = get_nearest_light(scene, object)) != NULL) {
        ar_light_shadow_render(light, shader_main);
    }
}

void ar_scene_objects_render(ar_scene_t *scene, ar_shader_t *shader, camera_t *camera, bool render_materials) {
    ar_object_t *objects = *((ar_object_t **)scene->objects.data);
    ar_objects_draw(objects, scene->objects.index, shader, camera, render_materials);
}

void ar_scene_render(ar_shader_t *shader_main, ar_scene_t *scene) {
    //int view_count = 0;
    
    //if (scene != NULL)
    //    view_count = scene->views_index;
    
    // draw main screen
    // set framebuffer to our 'default' framebuffer
    framebuffer_bind(NULL);
    
    //if (shader_depth != NULL) {
    unsigned i;
    ar_light_t *light;
    char str[48];
    for (i = 0; i < MAX_LIGHTS; i++) {
        if (i >= scene->lights.index) {
            sprintf(str, "pointLights[%d].shadow_map", i);
            ar_shader_set_int(shader_main, str, 4);
            continue;
        }
        light = ar_buffer_get(&scene->lights, i);
        // if shadows are setup, set shadow map in main shader
        if (light->use_shadows) {
            glActiveTexture(GL_TEXTURE4+light->point_shadow.shadow_map_id);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, light->point_shadow.framebuffer.texture->id);
            texture_bind(light->point_shadow.framebuffer.texture);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    handle_call(HANDLE_RENDER_MESHES, selected_camera);
    ar_shader_use(shader_main);
    ui_render();
}

void *ar_scene_attach(ar_scene_t *scene, ar_scene_attach_type_t type, void *ptr) {
    void *scene_object = NULL;
    if (type == AR_SCENE_ATTACH_SKYBOX) {
        //memcpy(&scene->skybox, ptr, sizeof(skybox_t));
        //scene->flags |= SCENE_ENABLE_SKYBOX;
        // TODO: add back
        //skybox_new(&scene->skybox.cubemap);
    }
    else if (type == AR_SCENE_ATTACH_LIGHT) {
        if (scene->lights.index == MAX_LIGHTS) {
            ar_log(AR_LOG_ERROR, "Already at max lights!\n", 0);
            return NULL;
        }
        
        scene_object = ar_buffer_push(&scene->lights, ptr);
    }
    else if (type == AR_SCENE_ATTACH_OBJECT) {
        scene_object = ar_buffer_push(&scene->objects, &ptr);
    }
    return scene_object;
}

void ar_scene_destroy(ar_scene_t *scene) {
    (void)scene;
    //int i;
    //for (i = 1; i < scene->views_index; i++) {
        //framebuffer_destroy(&scene->views[i].framebuffer);
    //}
}
