#include <f3d/engine/texture.h>
#include <f3d/engine/image.h>
#include <f3d/engine/log.h>
#include <f3d/engine/types.h>

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

texture_t texture_load(const char *path, int type) {
    texture_t tex;
    
    char ext[32];
    get_file_extension((char *)path, ext);
    
    tex.image = image_load(path, type);
    
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    
    if (tex.image.data == NULL) {
        log_msg(LOG_ERROR, "Error loading image\n", 0);
        return tex;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.image.width, tex.image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return tex;
}

void texture_destroy(texture_t *tex) {
    image_destroy(&tex->image);
    glDeleteTextures(1, &tex->id);
}
