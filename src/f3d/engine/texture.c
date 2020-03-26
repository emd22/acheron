#include <f3d/engine/texture.h>
#include <f3d/engine/image.h>
#include <f3d/engine/log.h>
#include <f3d/engine/types.h>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

void get_file_extension(char *path, char *buf) {
    int length = strlen(path);
    char *newb = path+length;
    int index = 0;
    // go from back of string to first '.'
    while (*(--newb) != '.');
    // skip '.'
    newb++;
    // go from '.' back to end of string, copying to buffer
    while ((buf[index++] = *(newb++)));
    // null terminate our string
    buf[index] = 0;
}

texture_t *texture_load(const char *path, int type) {
    texture_t *texture = malloc(sizeof(texture_t));
    
    char ext[32];
    get_file_extension((char *)path, ext);
    
    texture->image = image_load(path, type);
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    if (texture->image.data == NULL) {
        log_msg(LOG_ERROR, "Error loading image\n", 0);
        return texture;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->image.width, texture->image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return texture;
}

void texture_destroy(texture_t *texture) {
    if (texture == NULL)
        return;
    image_destroy(&texture->image);
    glDeleteTextures(1, &texture->id);
}
