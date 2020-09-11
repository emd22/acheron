#include <acheron/engine/asset/at_assetman.h>
#include <acheron/engine/asset/at_asset.h>
#include <acheron/engine/limits.h>

#include <string.h>
#include <stdlib.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/threads/cr_threadman.h>

void ar_assetman_init(void) {
    // pack buffer so we can reuse free'd slots
    ar_buffer_t *asset_buffer = ar_assetman_get_asset_buffer();
    ar_buffer_init(asset_buffer, AR_BUFFER_DYNAMIC, sizeof(ar_asset_t), 256, AR_BUFFER_PACK);

    ar_thread_t **thread = ar_assetman_get_thread();
    (*thread) = ar_thread_new(&ar_asset_thread_func, NULL);
}

void ar_assetman_destroy(void) {
    
    ar_thread_join(*ar_assetman_get_thread(), NULL);

    ar_buffer_t *asset_buffer = ar_assetman_get_asset_buffer();
    ar_buffer_destroy(asset_buffer);
}
