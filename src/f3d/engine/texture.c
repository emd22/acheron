#include <f3d/engine/texture.h>
#include <f3d/engine/image.h>
#include <f3d/engine/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MAX_TEXTURES 512

static texture_t textures[MAX_TEXTURES];
static int textures_index = 0;

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

    texture->data_type = TEXTURE_TYPE_RGB;
    texture->draw_type = TEXTURE_TYPE_RGBA;
    texture->bpp = 24;
    
    return texture;
}

void texture_init(texture_t *texture) {
    glGenTextures(1, &texture->id);
}

void texture_set_data(texture_t *texture, int width, int height, int bpp, unsigned char *data) {
    texture->bpp = bpp;
    texture->image.width = width;
    texture->image.height = height;
    texture->bpp = bpp;

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->draw_type, width, height, 0, texture->data_type, GL_UNSIGNED_BYTE, data);
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
        texture->image.bpp,
        texture->image.data 
    );
    
    // image data is already loaded into OpenGL, we no
    // longer need it in memory
    image_destroy(&texture->image);
    
    // TODO: change these settings, preferably from the struct and
    // an *_update function
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
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
