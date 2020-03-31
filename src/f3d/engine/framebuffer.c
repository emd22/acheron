#include <f3d/engine/framebuffer.h>

#include <GL/glew.h>
#include <GL/gl.h>

void framebuffer_init(framebuffer_t *fb) {
    const int width = 400;
    const int height = 400;

    glGenFramebuffers(1, &fb->fbo);
    glGenTextures(1, &fb->texture_id);
    glBindTexture(GL_TEXTURE_2D, fb->texture_id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenRenderbuffers(1, &fb->depth_id);
    glBindRenderbuffer(GL_RENDERBUFFER, gl->depth_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb->depth_id);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb->texture_id, 0);
}

void framebuffer_bind(framebuffer_t *fb) {
    if (fb == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    glViewport(0, 0, 400, 400);
}

void framebuffer_destroy(framebuffer_t *fb) {
    glDeleteFramebuffers(1, &fb->fbo);
}
