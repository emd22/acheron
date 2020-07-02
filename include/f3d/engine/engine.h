#ifndef F3D_ENGINE_H
#define F3D_ENGINE_H

#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/core/cr_handles.h>
#include <f3d/engine/core/cr_debug.h>
#include <f3d/engine/core/cr_window.h>
#include <f3d/engine/core/cr_log.h>
#include <f3d/engine/core/cr_time.h>

#include <f3d/engine/image/texture.h>

#include <f3d/engine/rendering/render.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/renderer/rr_shader.h>
#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/ui.h>

#include <f3d/engine/scene/sc_scene.h>
#include <f3d/engine/scene/sc_light.h>
#include <f3d/engine/scene/sc_skybox.h>
#include <f3d/engine/scene/sc_object.h>

#include <f3d/engine/object/material.h>

#include <stdbool.h>

const char *engine_get_opengl_error(void);
void engine_setup_signals(void);
void engine_render_wireframe(bool n);

#endif 
