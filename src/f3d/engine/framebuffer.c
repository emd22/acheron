#include <f3d/engine/framebuffer.h>
#include <f3d/engine/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

framebuffer_t framebuffer_new(int width, int height, int bpp) {
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
    fb.bpp = bpp;

    framebuffer_texture(&fb, GL_DEPTH_ATTACHMENT);
    return fb;
}

void framebuffer_bind(framebuffer_t *fb) {
    if (fb == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    //glViewport(fb->originx, fb->originy, fb->width, fb->height);
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
