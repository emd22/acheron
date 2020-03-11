#include <gl/texture.h>
#include <gl/image.h>
#include <gl/log.h>

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

texture_t texture_load(const char *path) {
    texture_t tex;
    
    char ext[32];
    get_file_extension((char *)path, ext);
    
    if (!strcmp(ext, "bmp") || !strcmp(ext, "BMP")) {
        tex.image = image_load(path, IMAGE_BMP);
    }
    else {
        log_msg(LOG_ERROR, "Unknown image type '%s'\n", ext);
        return tex;
    }
    
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
