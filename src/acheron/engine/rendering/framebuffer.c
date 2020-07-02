#include <acheron/engine/acheron.h>
#include <acheron/engine/rendering/framebuffer.h>
#include <acheron/engine/core/cr_log.h>
#include <acheron/engine/core/cr_window.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>

static ar_framebuffer_t *default_framebuffer = NULL;

void ar_framebuffer_set_default(ar_framebuffer_t *fb) {
    default_framebuffer = fb;
}

ar_framebuffer_t *ar_framebuffer_get_default(void) {
    return default_framebuffer;
}

ar_framebuffer_t ar_framebuffer_new(int width, int height, int attachment, bool depth_buffer) {
    ar_framebuffer_t fb;

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
    
    fb.texture_target = GL_TEXTURE_2D;
    
    ar_framebuffer_texture(&fb, attachment);
    fb.initialized = true;
    
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

void ar_framebuffer_generate_texture(ar_framebuffer_t *fb, int draw_type, int data_type, int type_size) {
    texture_t *tex = fb->texture;
    tex->draw_type = draw_type;
    tex->data_type = data_type;
    
    texture_set_data(tex, fb->width, fb->height, type_size, NULL);
    
    texture_set_parameter(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture_set_parameter(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture_set_parameter(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture_set_parameter(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void ar_framebuffer_bind(ar_framebuffer_t *fb) {
    if (fb == NULL) {
        int fbo = 0;
        ar_window_t *default_window = ar_instance_get_selected()->window;
        int width = default_window->width;
        int height = default_window->height;
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

void ar_framebuffer_texture(ar_framebuffer_t *fb, int attachment) {
    if (fb->texture_target == GL_TEXTURE_CUBE_MAP) {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fb->texture->id, 0);
        return;
    }
    glBindTexture(fb->texture_target, fb->texture->id);

    glTexParameteri(fb->texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(fb->texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(fb->texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(fb->texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, fb->texture_target, fb->texture->id, 0);
}

void ar_framebuffer_destroy(ar_framebuffer_t *fb) {
    if (fb == NULL || fb->initialized == false)
        return;
    fb->initialized = false;
    glDeleteFramebuffers(1, &fb->fbo);
    if (fb->texture->image.width != 0) {
        texture_destroy(fb->texture);
    }
}
