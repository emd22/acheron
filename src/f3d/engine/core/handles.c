#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>

int stub(void *p) {
    (void)p;
    return 0;
}

static handle_t handles[HANDLES_AMT];

void handles_init() {
    int i;
    for (i = 0; i < HANDLES_AMT; i++)
        handles[i] = &stub;
}

handle_t handle_get(int index) {
    return handles[index];
}

void handle_set(int index, handle_t handle) {
    handles[index] = handle;
}

int handle_call(int index, void *arg) {
    return handles[index](arg);
}
