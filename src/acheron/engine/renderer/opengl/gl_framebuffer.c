#include <acheron/engine/renderer/rr_framebuffer.h>
#include <acheron/engine/renderer/rr_texture.h>
#include <acheron/engine/acheron.h>
#include <acheron/engine/core/cr_log.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <stddef.h>

static bool framebuffer_check_complete(ar_framebuffer_t *fb) {
    if (fb == NULL)
        return false;

    ar_framebuffer_bind(fb);
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (status == GL_FRAMEBUFFER_COMPLETE)
        return true;

    switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            ar_log(AR_LOG_WARN, "incomplete attachment\n", 0);
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            ar_log(AR_LOG_WARN, "incomplete dimensions\n", 0);
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            ar_log(AR_LOG_WARN, "incomplete missing attachment\n", 0);
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            ar_log(AR_LOG_WARN, "fb unsupported\n", 0);
            break;
    }
    ar_log(AR_LOG_WARN, "Framebuffer completion: %d ::: %d\n", status, fb->id);
    return false;
}

ar_framebuffer_t ar_framebuffer_new(unsigned width, unsigned height) {
    ar_framebuffer_t fb;
    //ar_window_t *default_window = ar_instance_get_selected()->window;

    fb.texture = ar_texture_new(0);
    ar_texture_set_data(fb.texture, width, height, AR_TEXTURE_RGBA, AR_TEXTURE_BYTE, NULL);

    fb.x = 0;
    fb.y = 0;
    fb.width = width;
    fb.height = height;
    //fb.width  = default_window->width;
    //fb.height = default_window->height;
    glGenFramebuffers(1, &fb.id);
    ar_log(AR_LOG_INFO, "New framebuffer of size %dx%d with id %d\n", width, height, fb.id);
    ar_framebuffer_bind(&fb);

    ar_framebuffer_to_texture(&fb, fb.texture, AR_FRAMEBUFFER_COLOR0);
    framebuffer_check_complete(&fb);

    return fb;
}

void ar_framebuffer_bind(ar_framebuffer_t *fb) {
    if (fb == NULL) {
        ar_window_t *default_win = ar_instance_get_selected()->window;
        glViewport(0, 0, default_win->width, default_win->height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    //while (framebuffer_check_complete(fb));
    glViewport(fb->x, fb->y, fb->width, fb->height);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->id);
}

void ar_framebuffer_to_texture(ar_framebuffer_t *fb, ar_texture_t *texture, ar_framebuffer_attachment_type_t attachment) {
    const int mipmap_level = 0;
    (void)fb;

    const int gl_attach_table[] = {
        GL_COLOR_ATTACHMENT0, /* AR_FRAMEBUFFER_COLOR0 */
    };
    ar_log(AR_LOG_INFO, "Framebuffer %d to texture\n", fb->id);
    glFramebufferTexture2D(
            GL_FRAMEBUFFER, 
            gl_attach_table[attachment],
            ar_gl_texture_bind_type(texture->bind_type),
            texture->id,
            mipmap_level
    );
}

void ar_framebuffer_destroy(ar_framebuffer_t *fb) {
    if (fb == NULL)
        return;
    ar_log(AR_LOG_INFO, "Deleting framebuffer %d\n", fb->id);
    glDeleteFramebuffers(1, &fb->id);
}