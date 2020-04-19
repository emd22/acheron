#include <f3d/engine/image/texture.h>
#include <f3d/engine/image/image.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MAX_TEXTURES 512

static texture_t textures[MAX_TEXTURES];
static int textures_index = 0;
static texture_t *bound_texture = NULL;

texture_t *texture_new(void) {
    int index = textures_index++;
    // TODO: search for previously deleted textures, use a free slot
    texture_t *texture = &textures[index];
    
    if (textures_index > MAX_TEXTURES) {
        log_msg(LOG_WARN, "Maximum textures reached(%d > %d), resetting texture_index\n", textures_index, MAX_TEXTURES);
        // TODO: find a better way around this. we need a proper way to avoid
        // overwriting textures
        
        // reset textures_index to avoid going out of bounds
        textures_index = 0;
    }
    // redundant
    texture->index = index;

    texture->bind_type = GL_TEXTURE_2D;
    texture->draw_type = GL_RGB;
    texture->data_type = GL_RGB;
    handle_call(HANDLE_ON_TEXTURE_LOAD, texture);
    
    return texture;
}

void texture_bind(texture_t *texture) {
    if (bound_texture != NULL && bound_texture->id == texture->id)
        return;
    bound_texture = texture;
    glBindTexture(texture->bind_type, texture->id);
}

void texture_init(texture_t *texture) {
    glGenTextures(1, &texture->id);
}

void texture_set_parameter(texture_t *texture, int parameter, int value) {
    texture_bind(texture);
    glTexParameteri(texture->bind_type, parameter, value);
}

void texture_set_data(texture_t *texture, int width, int height, int type_size, unsigned char *data) {
    texture->image.width = width;
    texture->image.height = height;

    texture_bind(texture);

    glTexImage2D(texture->bind_type, 0, texture->draw_type, width, height, 0, texture->data_type, type_size, data);
}

texture_t *texture_load(texture_t *texture, const char *path, int type) {
    if (texture == NULL) {
        texture = texture_new();
        texture_init(texture);
    }
    
    texture->image = image_load(path, type);

    if (texture->image.data == NULL) {
        log_msg(LOG_ERROR, "Error loading image\n", 0);
        return texture;
    }
    
    texture_set_data(
        texture,
        texture->image.width, texture->image.height,
        GL_UNSIGNED_BYTE,
        texture->image.data
    );
    
    // image data is already loaded into OpenGL, we no
    // longer need it in memory
    image_destroy(&texture->image);
    
    texture_set_parameter(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture_set_parameter(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture_set_parameter(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture_set_parameter(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return texture;
}

texture_t *texture_load_data(texture_t *texture, const char *path, int type) {
    if (texture == NULL) {
        texture = texture_new();
        texture_init(texture);
    }
    
    texture->image = image_load(path, type);

    if (texture->image.data == NULL) {
        log_msg(LOG_ERROR, "Error loading image\n", 0);
        return texture;
    }
    
    return texture;
}

void texture_destroy(texture_t *texture) {
    if (texture == NULL)
        return;
    image_destroy(&texture->image);
    // delete texture from OpenGL
    glDeleteTextures(1, &texture->id);
}

void textures_cleanup(void) {
    int i;
    for (i = 0; i < textures_index; i++) {
        texture_destroy(&textures[i]);
    }
}
