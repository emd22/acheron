#include <acheron/engine/asset/at_assetman.h>
#include <acheron/engine/limits.h>

#include <string.h>
#include <stdlib.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/threads/cr_threadman.h>

ar_buffer_t asset_queue;

void ar_assetman_init(void) {
    ar_buffer_init(&asset_queue, AR_BUFFER_DYNAMIC, sizeof(ar_asset_t), 256, 0);
}

ar_asset_t *ar_asset_find_new(void) {
    return NULL;
}

void ar_assetman_destroy(void) {
    ar_buffer_destroy(&asset_queue);
}
