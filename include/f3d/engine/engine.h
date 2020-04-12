#ifndef F3D_ENGINE_H
#define F3D_ENGINE_H

#include <f3d/engine/texture.h>
#include <f3d/engine/window.h>
#include <f3d/engine/camera.h>
#include <f3d/engine/shader.h>
#include <f3d/engine/model.h>
#include <f3d/engine/material.h>
#include <f3d/engine/log.h>
#include <f3d/engine/time.h>
#include <f3d/engine/math.h>
#include <f3d/engine/light.h>
#include <f3d/engine/object.h>
#include <f3d/engine/shadows.h>
#include <f3d/engine/handles.h>

const char *engine_get_opengl_error(void);
void engine_setup_signals(void);

#endif 
