#include <f3d/engine/core/cr_handles.h>
#include <f3d/engine/core/cr_log.h>

#include <stdlib.h>

static handle_t handles[HANDLES_AMT];

static int stub(void *p);
static int stub_end(void *arg);

void handles_init() {
    int i;
    for (i = 0; i < HANDLES_AMT; i++)
        handles[i] = &stub;
    handle_set(HANDLE_END, &stub_end);
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

/*
 * Stub functions
 */

static int stub(void *p) {
    (void)p;
    return 0;
}

static int stub_end(void *arg) {
    (void)arg;
    exit(0);
}
