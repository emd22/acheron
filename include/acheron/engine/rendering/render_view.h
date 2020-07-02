#ifndef F3D_RENDER_VIEW_H
#define F3D_RENDER_VIEW_H

#include <acheron/engine/types.h>
#include <acheron/engine/rendering/camera.h>
#include <acheron/engine/rendering/framebuffer.h>

typedef struct {
    camera_t *camera;
    ar_framebuffer_t framebuffer;
    ar_vector2f_t dest_position, dest_dimensions;
    ar_vector2f_t src_position, src_dimensions;
} render_view_t;

#endif
