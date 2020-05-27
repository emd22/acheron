#include <f3d/engine/scene/light.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/core/log.h>

#include <stdio.h>
#include <string.h>

static light_t lights[MAX_LIGHTS];
static int lights_index = 0;
static light_t dummy;
static int shadow_map_id = 0;

int count_light_types(int type) {
    int i;
    int amt = 0;
    for (i = 0; i < lights_index-1; i++) {
        if (lights[i].type == type)
            amt++;
    }
    return amt;
}

light_t *light_get(buffer_t *lights, light_t *next, int type) {
    unsigned i;
    light_t *light;
    for (i = 0; i < lights->index; i++) {
        light = buffer_get(lights, i);
        if (light != next && light->type == type) {
            return light;
        }
    }
    return NULL;
}

light_t *light_new(int type) {
    memset(&dummy, 0, sizeof(light_t));
    dummy.type = LIGHT_DUMMY;
    if (lights_index == MAX_LIGHTS) {
        return &dummy;
    }
    light_t *light = &lights[lights_index++];
    // make sure all light data is NULL'd
    memset(light, 0, sizeof(light_t));
    
    // count amount of a type of light because of the shader having
    // multiple arrays for different lights
    light->index = count_light_types(type);
    light->type = type;
    light->use_shadows = false;
    
    if (type == LIGHT_POINT) {
        light->position = VEC3F(0.0f);
        light->ambient = VEC3F(0.10f);
        light->diffuse   = VEC3F(0.8f);
        light->specular  = VEC3F(1.0f);
        light->radius = 8.0f;
    }
    else if (type == LIGHT_DIRECTIONAL) {
        light->direction = (vector3f_t){-0.2, 0.8, -0.7};
        light->ambient   = VEC3F(0.02f);
        light->diffuse   = VEC3F(0.15f);
        light->specular  = VEC3F(0.3f);
    }
    
    return light;
}

void light_shadow_new(light_t *light, int width, int height) {
    if (light->type != LIGHT_POINT)
        return;
    light->point_shadow = shadows_point_init(light->position, width, height, light->radius);
    light->use_shadows = true;
    light->point_shadow.shadow_map_id = ++shadow_map_id;
}

void light_shadow_render(light_t *light, shader_t *shader_main) {
    if (light->use_shadows == false)
        return;
    shadows_point_render(&light->point_shadow, light->position, shader_main);
}

// TODO: replace this with something better
void light_init(light_t *light, shader_t *shader) {
    if (light == NULL || shader == NULL)
        return;
    if (light->type == LIGHT_DUMMY)
        return;
        
    char lightstr[48];
    if (light->type == LIGHT_DIRECTIONAL) {
        log_msg(LOG_INFO, "Initializing directional light (id: %d)\n", light->index);
        sprintf(lightstr, "dirLights[%d].direction", light->index);
        shader_set_vec3f(shader, lightstr, light->direction);

        sprintf(lightstr, "dirLights[%d].ambient", light->index);
        shader_set_vec3f(shader, lightstr, light->ambient);

        sprintf(lightstr, "dirLights[%d].diffuse", light->index);
        shader_set_vec3f(shader, lightstr, light->diffuse);

        sprintf(lightstr, "dirLights[%d].specular", light->index);
        shader_set_vec3f(shader, lightstr, light->specular);
    }
    else if (light->type == LIGHT_POINT) {
        log_msg(LOG_INFO, "Initializing point light (id: %d)\n", light->index);
        
        sprintf(lightstr, "pointLights[%d].position", light->index);
        shader_set_vec3f(shader, lightstr, light->position);

        sprintf(lightstr, "pointLights[%d].ambient", light->index);
        shader_set_vec3f(shader, lightstr, light->ambient);

        sprintf(lightstr, "pointLights[%d].diffuse", light->index);
        shader_set_vec3f(shader, lightstr, light->diffuse);

        sprintf(lightstr, "pointLights[%d].specular", light->index);
        shader_set_vec3f(shader, lightstr, light->specular);

        sprintf(lightstr, "pointLights[%d].radius", light->index);
        shader_set_float(shader, lightstr, light->radius);
        
        // Shadows
        sprintf(lightstr, "pointLights[%d].shadow_map", light->index);
        shader_set_int(shader, lightstr, 4+light->point_shadow.shadow_map_id);
        
        sprintf(lightstr, "pointLights[%d].shadow_far_plane", light->index);
        shader_set_float(shader, lightstr, light->point_shadow.far_plane);
        
        sprintf(lightstr, "pointLights[%d].shadows_enabled", light->index);
        shader_set_float(shader, lightstr, light->use_shadows);
    }
    else {
        log_msg(LOG_ERROR, "light type #%d not implemented\n", light->type);
    }
}

void light_update(light_t *light, shader_t *shader) {
    if (light == NULL || shader == NULL)
        return;
        
    char lightstr[32];
    if (light->type == LIGHT_DIRECTIONAL) {
        sprintf(lightstr, "dirLights[%d].direction", light->index);
        shader_set_vec3f(shader, lightstr, light->direction);
    }
    else if (light->type == LIGHT_POINT) {
        light->point_shadow.far_plane = light->radius;
        sprintf(lightstr, "pointLights[%d].position", light->index);
        shader_set_vec3f(shader, lightstr, light->position);
        
        if (light->use_shadows) {
            sprintf(lightstr, "pointLights[%d].shadow_map", light->index);
            shader_set_int(shader, lightstr, 4+light->point_shadow.shadow_map_id);
            
            sprintf(lightstr, "pointLights[%d].shadow_far_plane", light->index);
            shader_set_float(shader, lightstr, light->point_shadow.far_plane);
            
            sprintf(lightstr, "pointLights[%d].shadows_enabled", light->index);
            shader_set_float(shader, lightstr, light->use_shadows);
            
            shadows_point_update(&light->point_shadow, light->position);
            light_shadow_render(light, shader);
        }
    }
}
