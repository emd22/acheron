#ifndef ACHERON_H
#define ACHERON_H

#include <f3d/engine/renderer/renderer.h>
#include <f3d/engine/core/window.h>

#define AR_INSTANCE_GRAPHICS 0x01
#define AR_INSTANCE_ALL (AR_INSTANCE_GRAPHICS)

typedef struct {
    int flags;
    ar_window_t window;
    ar_renderer_instance_t renderer;
} ar_instance_t;

void sb_init(int flags);
void sb_destroy(void);

#endif
