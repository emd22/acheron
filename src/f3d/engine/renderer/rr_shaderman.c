#include <f3d/engine/renderer/rr_shaderman.h>
#include <f3d/engine/renderer/rr_shader.h>

#include <f3d/engine/limits.h>
#include <f3d/engine/core/memory/mm_memory.h>
#include <f3d/engine/core/cr_log.h>

#include <f3d/engine/types.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

static ar_buffer_t shader_buffer;

ar_shader_t *ar_shader_new(const char *name) {
    if (!ar_buffer_is_initialized(&shader_buffer)) {
        ar_buffer_init(&shader_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_shader_t), 8, 0);
    }
    ar_shader_t *shader = ar_buffer_new_item(&shader_buffer);
    shader->hash = util_hash_str(name);
    
    shader->vertex = AR_SHADER_NONE;
    shader->fragment = AR_SHADER_NONE;
    shader->geometry = AR_SHADER_NONE;
    shader->program = AR_SHADER_NONE;
    shader->uniform_index = 0;
    
    strcpy(shader->name, name);
    return shader;
}

ar_shader_t *ar_shader_get(const char *name) {
    unsigned i;
    hash_t hash = util_hash_str(name);
    ar_shader_t *shader;
    for (i = 0; i < shader_buffer.index; i++) {
        shader = ar_buffer_get(&shader_buffer, i);
        if (hash == shader->hash)
            return shader;
    }
    return NULL;
}

void ar_shader_buffer_destroy(void) {
    ar_buffer_destroy(&shader_buffer);
}

