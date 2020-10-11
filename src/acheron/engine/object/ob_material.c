#include <acheron/engine/object/ob_material.h>
#include <acheron/engine/renderer/rr_shaderman.h>

#include <acheron/engine/core/memory/mm_memory.h>

static ar_buffer_t material_buffer;

static ar_material_t dummy_material(void) {
    ar_material_t dummy;

    dummy.diffuse_texture = NULL;
    dummy.specular_texture = NULL;
    dummy.normal_texture = NULL;
    dummy.shininess = 32.0f;

    return dummy;  
}

ar_material_t *ar_material_new(void) {
    if (!ar_buffer_is_initialized(&material_buffer)) {
        ar_buffer_init(&material_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_material_t), 64, AR_BUFFER_PACK);
    }

    ar_material_t *material = ar_buffer_new_item(&material_buffer);
    material->shininess = 32.0f;
    material->diffuse_texture  = NULL;
    material->normal_texture   = NULL;
    material->specular_texture = NULL;
    return material;
}

void ar_material_set_active(ar_material_t *material) {
    ar_shader_t *shader = ar_shaderman_get_render_shader();

    if (material == NULL) {
        // setup dummy material
        ar_material_t material = dummy_material();
        ar_material_set_active(&material);
        return;
    }

    int ti = 0;

    if (material->diffuse_texture) {
        ar_texture_bind_to(material->diffuse_texture, AR_TEXTURE_UNIT0);
        ti = 1; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.use_diffuse", &ti);
        ti = 0; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.diffuse", &ti);
    }
    
    if (material->specular_texture) {
        ar_texture_bind_to(material->diffuse_texture, AR_TEXTURE_UNIT1);
        ti = 1; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.use_specularmap", &ti);
        ti = 1; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.specular", &ti);
    }

    if (material->normal_texture) {
        ar_texture_bind_to(material->diffuse_texture, AR_TEXTURE_UNIT2);
        ti = 1; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.use_normalmap", &ti);
        ti = 2; ar_shader_set_uniform(shader, AR_SHADER_INT, "material.normal", &ti);
    }
    ar_shader_set_uniform(shader, AR_SHADER_FLOAT, "material.shininess", &material->shininess);
}


void ar_material_buffer_destroy(void) {
    ar_buffer_destroy(&material_buffer);
}