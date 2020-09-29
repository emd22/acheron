#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/core/memory/mm_memory.h>

#ifdef AR_USE_OPENGL

#include <GL/glew.h>
#include <GL/gl.h>

static ar_buffer_t texture_buffer;
static ar_texture_t *bound_texture = NULL;

ar_texture_t *ar_texture_new(void) {
    if (!ar_buffer_is_initialized(&texture_buffer)) {
        ar_buffer_init(&texture_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_texture_t), 64, AR_BUFFER_PACK);
    }
    ar_texture_t *texture = ar_buffer_new_item(&texture_buffer);
    texture->bind_type = AR_TEXTURE_2D;
    texture->draw_type = AR_TEXTURE_RGBA;
    texture->data_type = AR_TEXTURE_RGBA;
    texture->data_width = AR_TEXTURE_BYTE;
    texture->width = 0;
    texture->height = 0;
    texture->lod = 0;
    texture->image.type = ARI_TYPE_UNKNOWN;
    glGenTextures(1, &texture->id);

    // functions
    texture->update = &ar_texture_update;

    return texture;
}

void ar_texture_set_parameter(ar_texture_t *texture, ar_texture_parameter_t param, ar_texture_parameter_value_t value) {
    const unsigned gl_param_table[] = {
        GL_TEXTURE_MIN_FILTER, // AR_TEXTURE_MIN_FILTER
        GL_TEXTURE_MAG_FILTER, // AR_TEXTURE_MIN_FILTER
        GL_TEXTURE_WRAP_S,     // AR_TEXTURE_WRAP_S
        GL_TEXTURE_WRAP_T      // AR_TEXTURE_WRAP_T
    };
    const unsigned gl_value_table[] = {
        GL_LINEAR,               // AR_TEXTURE_LINEAR
        GL_LINEAR_MIPMAP_LINEAR, // AR_TEXTURE_LINEAR_MIPMAP
        GL_REPEAT                // AR_TEXTURE_REPEAT
    };

    // map values to conversion tables
    const unsigned gl_param = gl_param_table[param];
    const unsigned gl_value = gl_value_table[value];

    // bind our texture for OpenGL, set parameter for texture unit
    ar_texture_bind(texture);
    glTexParameteri(texture->bind_type, gl_param, gl_value);
}

ari_image_t *ar_texture_get_image(ar_texture_t *texture) {
    if (texture)
        return &texture->image;
    return NULL;
}

void ar_texture_update(ar_texture_t *texture) {
    if (texture->image.type == ARI_TYPE_UNKNOWN)
        return;
    const int image_width = texture->image.width;
    const int image_height = texture->image.height;
    ar_texture_set_data(
        texture, 
        image_width, image_height, 
        texture->data_type, texture->data_width, 
        texture->image.data
    );
}

void ar_texture_set_data(
    ar_texture_t *texture, int width, int height, 
    ar_texture_data_type_t data_type,
    ar_texture_data_width_t data_width, void *data
){
    const unsigned gl_data_width_table[] = {
        GL_UNSIGNED_BYTE,  // AR_TEXTURE_BYTE
        GL_UNSIGNED_SHORT, // AR_TEXTURE_SHORT
        GL_UNSIGNED_INT,   // AR_TEXTURE_INT
    };

    texture->data_width = data_width;
    texture->data_type = data_type;
    texture->width = width;
    texture->height = height;

    const unsigned gl_data_width = gl_data_width_table[texture->data_width];

    ar_texture_bind(texture);
    glTexImage2D(
        GL_TEXTURE_2D, 
        texture->lod, 
        texture->draw_type, 
        width, height, 0, 
        texture->data_type,
        gl_data_width,
        data
    );
}

void ar_texture_bind(ar_texture_t *texture) {
    if (bound_texture && bound_texture->id == texture->id)
        return; // already bound, just return
    
    glBindTexture(texture->bind_type, texture->id);
}

void ar_texture_destroy(ar_texture_t *texture) {
    if (texture == NULL)
        return;
    glDeleteTextures(1, &texture->id);
    
    if (texture->image.type != ARI_TYPE_UNKNOWN)
        ar_image_destroy(&texture->image);

    ar_buffer_item_free(&texture_buffer, texture);
}

void ar_texture_buffer_destroy(void) {
    ar_buffer_destroy(&texture_buffer);
}

#endif