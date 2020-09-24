#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/core/memory/mm_memory.h>

#ifdef AR_USE_OPENGL

#include <GL/glew.h>
#include <GL/gl.h>

static ar_buffer_t texture_buffer;

ar_texture_t *ar_texture_new(void) {
    if (!ar_buffer_is_initialized(&texture_buffer)) {
        ar_buffer_init(&texture_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_texture_t), 64, AR_BUFFER_PACK);
    }

}

void ar_texture_destroy(ar_texture_t *texture) {
    if (texture == NULL)
        return;
    glDeleteTextures(1, &texture->id);
    ar_image_destroy(texture->image);

    ar_buffer_item_free(&texture_buffer, ar_buffer_item_get_index(texture));
}

void ar_texture_buffer_destroy() {
    ar_buffer_destroy(&texture_buffer);
}

#endif