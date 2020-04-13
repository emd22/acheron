#include <f3d/engine/material.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/log.h>

#include <string.h>

#define MATERIALS_MAX 128

static material_t materials[MATERIALS_MAX];
static int materials_index = 0; 

material_t *material_new(material_t material) {
    material_t *mat = &materials[materials_index++];
    memcpy(mat, &material, sizeof(material_t));
    log_msg(LOG_INFO, "Creating new material '%s'\n", mat->name);
    return mat;
}

void material_update(material_t *mat, shader_t *shader) {
    // TODO: flat colours, etc.
    if (mat->diffuse == NULL) {
        log_msg(LOG_ERROR, "Diffuse texture for '%s' is NULL!\n", mat->name);
        return;
    }
    // assign diffuse to texture0 in fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->diffuse->id);
    // assign specular
    if (mat->specular != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mat->specular->id);
    }
    // assign normal
    if (mat->normal != NULL) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mat->normal->id);
    }
    else {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mat->diffuse->id);
    }
    shader_set_int(shader, "material.use_normalmap", mat->use_normals);
    shader_set_int(shader, "material.diffuse", mat->diffuse_id);
    shader_set_int(shader, "material.specular", mat->specular_id);
    shader_set_int(shader, "material.normal", mat->normal_id);
    shader_set_float(shader, "material.shininess", mat->shininess);
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
        
    texture_destroy(material->diffuse);
    texture_destroy(material->specular);
    texture_destroy(material->normal);
    
    material->name[0] = '*';
}
