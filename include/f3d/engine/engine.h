#ifndef F3D_ENGINE_H
#define F3D_ENGINE_H

#include <f3d/engine/core/window.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/time.h>
#include <f3d/engine/math/mt_math.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/debug.h>

#include <f3d/engine/image/texture.h>

#include <f3d/engine/rendering/render.h>
#include <f3d/engine/rendering/camera.h>
#include <f3d/engine/renderer/rr_shader.h>
#include <f3d/engine/rendering/shadows.h>
#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/ui.h>

#include <f3d/engine/scene/scene.h>
#include <f3d/engine/scene/light.h>
#include <f3d/engine/scene/skybox.h>
#include <f3d/engine/scene/object.h>

#include <f3d/engine/object/material.h>

#include <stdbool.h>

const char *engine_get_opengl_error(void);
void engine_setup_signals(void);
void engine_render_wireframe(bool n);

#endif 
