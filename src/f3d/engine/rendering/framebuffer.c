#include <f3d/engine/rendering/framebuffer.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/window.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

framebuffer_t *default_framebuffer = NULL;

framebuffer_t framebuffer_new(int width, int height, int attachment, bool depth_buffer) {
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
    
    if (depth_buffer) {
        glGenRenderbuffers(1, &fb.depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, fb.depth_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb.depth_buffer);
    }
    else
        fb.depth_buffer = 0;
    
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
        int fbo = 0;
        int width = default_window->width, height = default_window->height;
        if (default_framebuffer != NULL) {
            width = default_framebuffer->width;
            height = default_framebuffer->height;
            fbo = default_framebuffer->fbo;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
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
}

void framebuffer_destroy(framebuffer_t *fb) {
    if (fb == NULL)
        return;
    glDeleteFramebuffers(1, &fb->fbo);
    if (fb->texture->image.width != 0) {
        texture_destroy(fb->texture);
    }
}
