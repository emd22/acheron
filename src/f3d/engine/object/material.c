#include <f3d/engine/object/material.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>

#include <string.h>

static material_t materials[MATERIALS_MAX];
static int materials_index = 0; 

material_t *material_new(material_t material) {
    material_t *mat = &materials[materials_index++];
    memcpy(mat, &material, sizeof(material_t));
    mat->hash = util_hash_str(mat->name);
    log_msg(LOG_INFO, "Creating new material '%s'\n", mat->name);
    return mat;
}

void material_update(material_t *mat, shader_t *shader) {
    bool use_material = false;
    material_t dummy;
    // TODO: flat colours, etc.
    if (mat == NULL) {
        dummy.shininess = 32.0f;
        dummy.use_normals = false;
        dummy.use_specularmap = false;
        mat = &dummy;
        goto setup_material;
    }
    if (mat->diffuse != NULL) {
        // assign diffuse to texture0 in fragment shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mat->diffuse->id);
        use_material = true;
    }
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
    (void)use_material;
setup_material:;
    shader_set_int(shader, "material.use_normalmap", mat->use_normals);
    shader_set_int(shader, "material.use_specularmap", mat->use_specularmap);
    shader_set_int(shader, "material.use_diffuse", use_material);
    shader_set_int(shader, "material.diffuse", 0);
    shader_set_int(shader, "material.specular", 1);
    shader_set_int(shader, "material.normal", 2);
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
