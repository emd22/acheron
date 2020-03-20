#include <f3d/engine/material.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/log.h>

#include <string.h>

#define MATERIALS_MAX 64

static material_t materials[MATERIALS_MAX];
static int materials_index = 0; 

material_t *material_new(material_t material) {
    material_t *mat = &materials[materials_index++];
    memcpy(mat, &material, sizeof(material_t));
    log_msg(LOG_INFO, "Creating new material '%s'\n", mat->name);
    return mat;
}

void material_update(material_t *mat, unsigned shaderid) {
    // assign image to texture0 in fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse.id);
    // assign specular to texture1 in fragment shader
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat->specular.id);

    shader_set_int(shaderid, "material.diffuse", mat->diffuse_id);
    shader_set_int(shaderid, "material.specular", mat->specular_id);
    shader_set_float(shaderid, "material.shininess", mat->shininess);
}

material_t *material_get(const char *name) {
    int i;
    for (i = 0; i < materials_index; i++) {
        if (!strcmp(materials[i].name, name))
            return &(materials[i]);
    }
    // replace with 'default' material
    return NULL;
}

void material_destroy(material_t *material) {
    log_msg(LOG_INFO, "Deleting material '%s'\n", material->name);
    texture_destroy(&(material->diffuse));
    texture_destroy(&(material->specular));
    material->name[0] = '*';
}
