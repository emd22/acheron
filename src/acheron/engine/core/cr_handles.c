#include <acheron/engine/core/cr_handles.h>
#include <acheron/engine/core/cr_log.h>

#include <stdlib.h>

static ar_handle_t handles[AR_HANDLES_AMT];

static int stub(void *p);
static int stub_end(void *arg);

void ar_handles_init(void) {
    int i;
    for (i = 0; i < AR_HANDLES_AMT; i++)
        handles[i] = &stub;
    ar_handle_set(AR_HANDLE_END, &stub_end);
}

ar_handle_t ar_handle_get(ar_handle_type_t handle_type) {
    return handles[handle_type];
}

void ar_handle_set(ar_handle_type_t handle_type, ar_handle_t handle) {
    handles[handle_type] = handle;
}

int ar_handle_call(ar_handle_type_t handle_type, void *arg) {
    return handles[handle_type](arg);
}

/*
 * Stub functions
 */

static int stub(void *arg) {
    (void)arg;
    return 0;
}

static int stub_end(void *arg) {
    (void)arg;
    exit(0);
}
