#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/window.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

framebuffer_t framebuffer_new(int width, int height, int attachment) {
    framebuffer_t fb;

    glGenFramebuffers(1, &fb.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
    
    //fb.texture->id = FRAMEBUFFER_NO_TEXTURE;
    fb.texture = texture_new();
    fb.texture->image.width = width;
    fb.texture->image.height = height;
    texture_init(fb.texture);
    
    fb.width = width;
    fb.height = height;
    fb.originx = 0;
    fb.originy = 0;
    
    framebuffer_texture(&fb, attachment);
    
    return fb;
}

void framebuffer_generate_texture(framebuffer_t *fb, int draw_type, int data_type, int type_size) {
    texture_t *tex = fb->texture;
    tex->draw_type = draw_type;
    tex->data_type = data_type;
    
    texture_set_data(tex, fb->width, fb->height, type_size, NULL);
    
    texture_set_parameter(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture_set_parameter(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture_set_parameter(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture_set_parameter(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void framebuffer_bind(framebuffer_t *fb) {
    if (fb == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, default_window->width, default_window->height);
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    glViewport(fb->originx, fb->originy, fb->width, fb->height);
}

void framebuffer_texture(framebuffer_t *fb, int attachment) {
    glBindTexture(GL_TEXTURE_2D, fb->texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, fb->texture->id, 0);

    glDrawBuffer(GL_NONE);
}

void framebuffer_destroy(framebuffer_t *fb) {
    glDeleteFramebuffers(1, &fb->fbo);
    if (fb->texture->image.width != 0) {
        texture_destroy(fb->texture);
    }
}
