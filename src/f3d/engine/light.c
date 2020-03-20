#include <f3d/engine/light.h>
#include <f3d/engine/log.h>
#include <f3d/engine/shader.h>

#include <stdio.h>

// NOTE: edit these macros in shader as well
#define MAX_DIR_LIGHTS   8
#define MAX_POINT_LIGHTS 8
#define MAX_SPOTLIGHTS   8

static light_t lights[MAX_LIGHTS];
static int lights_index = 0;

int count_light_types(int type) {
    int i;
    int amt = 0;
    for (i = 0; i < lights_index; i++) {
        if (lights[i].type == type)
            amt++;
    }
    return amt;
}

light_t *light_new(int type) {
    if (lights_index == MAX_LIGHTS) {
        log_msg(LOG_ERROR, "Light limit (%d) reached\n", MAX_LIGHTS);
        return NULL;
    }
    light_t *light = &lights[lights_index++];
    light->index = count_light_types(type);
    light->type = type;
    return light;
}

void light_init(light_t *light, unsigned shaderid) {
    if (light == NULL)
        return;
        
    char lightstr[32];
    if (light->type == LIGHT_DIRECTIONAL) {
        sprintf(lightstr, "dirLights[%d].direction", light->index);
        shader_set_vec3f(shaderid, lightstr, light->direction);

        sprintf(lightstr, "dirLights[%d].ambient", light->index);
        shader_set_vec3f(shaderid, lightstr, light->ambient);

        sprintf(lightstr, "dirLights[%d].diffuse", light->index);
        shader_set_vec3f(shaderid, lightstr, light->diffuse);

        sprintf(lightstr, "dirLights[%d].specular", light->index);
        shader_set_vec3f(shaderid, lightstr, light->specular);
    }
}
