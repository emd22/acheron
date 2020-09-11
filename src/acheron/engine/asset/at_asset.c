#include <acheron/engine/limits.h>
#include <acheron/engine/asset/at_asset.h>
#include <acheron/engine/core/threads/cr_threadman.h>
#include <acheron/engine/core/cr_log.h>
#include <ar_image/ar_image.h>

#include <stdlib.h>

#define AR_ASSET_THREAD_SLEEP 300

static void asset_load(ar_asset_queue_item_t *item);
static void asset_destroy(ar_asset_queue_item_t *item);
void ar_asset_check_queue(void);

static ar_buffer_t asset_buffer;
static ar_buffer_t asset_queue;
static ar_thread_t *asset_thread;
static bool asset_thread_running = true;

ar_thread_t **ar_assetman_get_thread(void) {
    return &asset_thread;
}

ar_buffer_t *ar_assetman_get_asset_buffer(void) {
    return &asset_buffer;
}

void ar_assetman_thread_stop(void) {
    asset_thread_running = false;
}

void *ar_asset_thread_func(void *arg) {
    (void)arg;
    while (asset_thread_running) {
        ar_asset_check_queue();
        ar_thread_sleep(AR_ASSET_THREAD_SLEEP);
    }
    return NULL;
}

ar_asset_t *ar_asset_load(ar_asset_type_t type, char *path) {
    if (!ar_buffer_is_initialized(&asset_buffer)) {
        ar_log(AR_LOG_FATAL, "Asset buffer not initialized!\n", 0);
        exit(1);
    }
    ar_asset_t *asset = ar_buffer_new_item(&asset_buffer);
    asset->type = type;
    asset->data.raw = NULL;

    ar_asset_queue_item_t *item = ar_buffer_new_item(&asset_queue);
    item->operation = AR_ASSET_OP_LOAD;
    item->asset = asset;
    item->path = path;

    return asset;
}

void ar_asset_check_queue(void) {
    unsigned i;
    ar_asset_queue_item_t *item;
    for (i = 0; i < asset_queue.index; i++) {
        item = ar_buffer_get(&asset_buffer, i);
        if (item->operation == AR_ASSET_OP_LOAD) {
            asset_load(item);
            ar_buffer_item_free(&asset_queue, i);
        }
        else if (item->operation == AR_ASSET_OP_FREE) {
            asset_destroy(item);
            ar_buffer_item_free(&asset_queue, i);
        }
    }
}

void ar_asset_wait(ar_asset_t *asset) {
    while (asset->status == AR_ASSET_QUEUED)
        ar_thread_sleep(150);
}

void ar_asset_destroy(ar_asset_t *asset) {
    long index = ar_buffer_get_item_index(&asset_queue, asset);
    if (index < 0) {
        ar_log(AR_LOG_ERROR, "Asset not in asset list!\n", 0);
        return;
    }
    ar_buffer_item_free(&asset_queue, index);
}

static void asset_load(ar_asset_queue_item_t *item) {
    (void)item;
}

static void asset_destroy(ar_asset_queue_item_t *item) {
    (void)item;
}

#if 0
void ar_asset_load_image(const char *path, ar_asset_t *asset, ari_image_type_t image_type) {
    ar_image_load(path, &asset->data.image, image_type);
}

ar_asset_t ar_asset_load(const char *path, ar_asset_type_t type) {
    if (type == AR_ASSET_IMAGE) {
        ar_image_load(path, &asset->data.image, ARI_TYPE_AUTO);
        asset->type = AR_ASSET_IMAGE;
    }
    else if (type == AR_ASSET_MODEL) {
        
    }
     
}
#endif
