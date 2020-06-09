#ifndef F3D_RENDER_VIEW_H
#define F3D_RENDER_VIEW_H

#include <f3d/engine/types.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/rendering/framebuffer.h>

typedef struct {
    camera_t *camera;
    ar_framebuffer_t framebuffer;
    vector2f_t dest_position, dest_dimensions;
    vector2f_t src_position, src_dimensions;
} render_view_t;

#endif
