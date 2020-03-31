#include <f3d/engine/texture.h>
#include <f3d/engine/image.h>
#include <f3d/engine/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define MAX_TEXTURES 512

static texture_t *textures[MAX_TEXTURES];
static int textures_index = 0;

texture_t *texture_load(const char *path, int type, int flags) {
    // allocate texture as a pointer, save pointer to a list to destroy or
    // 'cull' later
    texture_t *texture = malloc(sizeof(texture_t));
    
    if (textures_index > MAX_TEXTURES) {
        log_msg(LOG_ERROR, "Maximum textures reached(%d > %d), resetting texture_index\n", textures_index, MAX_TEXTURES);
        // TODO: find a better way around this. We may not need more than
        // 512 textures on screen at once, but we still need a proper way to avoid
        // losing memory
        
        // reset textures_index to avoid going out of bounds
        textures_index = 0;
    }
    
    int index = textures_index++;
    
    texture->index = index;
    textures[index] = texture;
    
    texture->image = image_load(path, type);
    
    // generate texture on graphics card
    glGenTextures(1, &texture->id);
    // bind to GL_TEXTURE_2D temporarily
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    if (texture->image.data == NULL) {
        log_msg(LOG_ERROR, "Error loading image\n", 0);
        return texture;
    }
    
    // copy our image data to GL_TEXTURE_2D, which is linked to texture->id
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->image.width, texture->image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->image.data);
    
    // the image data has already been loaded into VRAM, we can free it
    // from system memory now.
    // we will probably never need to use this flag, but it could prove useful
    // to modify the texture data on the fly and rebind to a new texture object
    if (!(flags & TEXTURE_KEEP_DATA))
        image_destroy(&texture->image);
    
    // TODO: change these settings, preferably from the struct and
    // an *_update function
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    texture->flags = flags;
    
    return texture;
}

void texture_destroy(texture_t *texture) {
    if (texture == NULL)
        return;
    // image_destroy handles if it has already been destroyed
    image_destroy(&texture->image);
    // delete texture from vram
    glDeleteTextures(1, &texture->id);
}

void textures_cleanup(void) {
    // loop through all texture objects and destroy each. this approach cleans up the
    // game code a lot, so we dont pepper texture_destroy calls everywhere. we keep the
    // individual function in case we need to destroy the texture earlier than expected, or
    // have no use for it anymore.
    int i;
    for (i = 0; i < textures_index; i++) {
        texture_destroy(textures[i]);
    }
}
