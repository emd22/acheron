#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/core/memory/mm_memory.h>

#ifdef AR_USE_OPENGL

#include <GL/glew.h>
#include <GL/gl.h>

#define ar_gl_draw_type(data_type) ar_gl_data_type(data_type)

static ar_buffer_t texture_buffer;
static ar_texture_t *bound_texture = NULL;

static ar_texture_unit_t active_texture = AR_TEXTURE_UNIT_NONE;

unsigned ar_gl_texture_bind_type(ar_texture_bind_type_t type) {
    static unsigned gl_bind_type_table[] = {
        GL_TEXTURE_2D, // AR_TEXTURE_2D
        GL_TEXTURE_CUBE_MAP, // AR_TEXTURE_CUBEMAP
    };
    return gl_bind_type_table[type];
}

static unsigned ar_gl_data_width(ar_texture_data_width_t width) {
    const unsigned gl_data_width_table[] = {
        GL_UNSIGNED_BYTE,  // AR_TEXTURE_BYTE
        GL_UNSIGNED_SHORT, // AR_TEXTURE_SHORT
        GL_UNSIGNED_INT,   // AR_TEXTURE_INT
    };
    return gl_data_width_table[width];
}

unsigned ar_gl_data_type(ar_texture_data_type_t data_type) {
    const unsigned gl_data_type_table[] = {
        GL_RGB,  /* AR_TEXTURE_RGB */
        GL_RGBA, /* AR_TEXTURE_RGBA */
        GL_BGR,  /* AR_TEXTURE_BGR */
        GL_BGRA  /* AR_TEXTURE_BGRA */
    };
    return gl_data_type_table[data_type];
}

ar_texture_t *ar_texture_new(int flags) {
    if (!ar_buffer_is_initialized(&texture_buffer)) {
        ar_buffer_init(&texture_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_texture_t), 64, AR_BUFFER_PACK);
    }
    ar_texture_t *texture = ar_buffer_new_item(&texture_buffer);
    texture->bind_type = AR_TEXTURE_2D;
    texture->draw_type = AR_TEXTURE_RGBA;
    texture->data_type = AR_TEXTURE_RGBA;
    texture->data_width = AR_TEXTURE_BYTE;
    texture->texture_unit = AR_TEXTURE_UNIT0;

    texture->width = 0;
    texture->height = 0;
    texture->lod = 0;
    texture->flags = flags;
    
    texture->image.type = ARI_TYPE_UNKNOWN;
    glGenTextures(1, &texture->id);

    // functions
    texture->update = &ar_texture_update;
    texture->bind   = &ar_texture_bind;
    texture->bind_to = &ar_texture_bind_to;

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

    const unsigned gl_bind_type = ar_gl_texture_bind_type(texture->bind_type);
    glTexParameteri(gl_bind_type, gl_param, gl_value);
}

ari_image_t *ar_texture_get_image(ar_texture_t *texture) {
    if (texture)
        return &texture->image;
    return NULL;
}

void ar_texture_update(ar_texture_t *texture) {
    if (texture->image.type == ARI_TYPE_UNKNOWN) {
        return;
    }
    const int image_width = texture->image.width;
    const int image_height = texture->image.height;
    texture->width = image_width;
    texture->height = image_height;

    ar_texture_set_data(
        texture, 
        image_width, image_height, 
        texture->data_type, texture->data_width, 
        texture->image.data
    );
}

/*
void ar_texture_update_data(ar_texture_t *texture) {
    const unsigned gl_bind_type = ar_gl_texture_bind_type(texture->bind_type);
    glTexSubImage2D(
        gl_bind_type,
        texture->lod,
        0, 0,
        texture->width,
        texture->height,
        texture->data_type,
        ar_gl_data_width(texture->data_width),
        texture->image.data
    );
}
*/

void ar_texture_copy_to(ar_texture_t *dest, ar_texture_t *src, int xoff, int yoff, int width, int height) {
    ar_texture_bind(dest);

    glTexSubImage2D(
        ar_gl_texture_bind_type(dest->bind_type),
        dest->lod,
        xoff, yoff,
        width, height,
        ar_gl_draw_type(src->draw_type),
        ar_gl_data_width(src->data_width),
        src->image.data
    );
}

void ar_texture_set_data(
    ar_texture_t *texture, int width, int height, 
    ar_texture_data_type_t data_type,
    ar_texture_data_width_t data_width, void *data
){
    texture->data_width = data_width;
    texture->data_type = data_type;
    texture->width = width;
    texture->height = height;

    const unsigned gl_data_width = ar_gl_data_width(texture->data_width);
    const unsigned gl_bind_type  = ar_gl_texture_bind_type(texture->bind_type);

    const unsigned gl_draw_type = ar_gl_data_type(texture->draw_type);
    const unsigned gl_data_type = ar_gl_data_type(texture->data_type);

    ar_texture_bind(texture);
    glTexImage2D(
        gl_bind_type, 
        texture->lod, 
        gl_draw_type, 
        width, height, 0, 
        gl_data_type,
        gl_data_width,
        data
    );
    glGenerateMipmap(gl_bind_type);

    if (texture->flags & AR_TEXTURE_MIPMAP) {
        ar_texture_set_parameter(texture, AR_TEXTURE_MIN_FILTER, AR_TEXTURE_LINEAR_MIPMAP);
    }
    else {
        ar_texture_set_parameter(texture, AR_TEXTURE_MIN_FILTER, AR_TEXTURE_LINEAR);
    }
}

void ar_texture_bind_to(ar_texture_t *texture, ar_texture_unit_t texture_unit) {
    if (bound_texture && bound_texture->id == texture->id) {
        return; // already bound, just return
    }

    // check if the texture is already active
    if (active_texture != texture_unit && texture_unit != AR_TEXTURE_UNIT_NONE) {
        // acheron's texture units are just indices, so we just add to GL_TEXTURE0
        const unsigned gl_texture_c = GL_TEXTURE0+texture->texture_unit;
        active_texture = texture->texture_unit;
     
        // activate the texture unit
        glActiveTexture(gl_texture_c);
    }

    glBindTexture(ar_gl_texture_bind_type(texture->bind_type), texture->id);
}

void ar_texture_bind(ar_texture_t *texture) {
    ar_texture_unit_t unit;
    unit = texture->texture_unit;
    if (unit == AR_TEXTURE_UNIT_NONE)
        unit = AR_TEXTURE_UNIT0; // TODO: don't overwrite texture0
    
    ar_texture_bind_to(texture, unit);
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