#include <acheron/engine/limits.h>
#include <acheron/engine/asset/at_asset.h>
#include <acheron/engine/core/threads/cr_threadman.h>
#include <acheron/engine/core/cr_log.h>
#include <ar_image/ar_image.h>

#include <stdlib.h>

#define AR_ASSET_SLEEP_TIME 400

static void asset_load(ar_asset_queue_item_t *item);
static void asset_destroy(ar_asset_queue_item_t *item);
void ar_asset_check_queue(void);

static ar_buffer_t asset_buffer;
static ar_buffer_t asset_queue;
static ar_thread_t *asset_thread;
static ar_asset_thread_state_t asset_thread_state = AR_ASSET_THREAD_RUNNING;

ar_thread_t **ar_assetman_get_thread(void) {
    return &asset_thread;
}

ar_buffer_t *ar_assetman_get_queue_buffer(void) {
    return &asset_queue;
}

ar_buffer_t *ar_assetman_get_asset_buffer(void) {
    return &asset_buffer;
}

void ar_assetman_thread_set_state(ar_asset_thread_state_t state) {
    asset_thread_state = state;
}

void *ar_asset_thread_func(void *arg) {
    (void)arg;
    while (asset_thread_state) {
        while (asset_thread_state == AR_ASSET_THREAD_WAIT) {
            ar_thread_sleep(AR_ASSET_SLEEP_TIME);
        }
        ar_asset_check_queue();
        ar_thread_sleep(AR_ASSET_SLEEP_TIME);
    }
    return NULL;
}

ar_asset_t *ar_asset_load(ar_asset_type_t type, char *path) {
    if (!ar_buffer_is_initialized(&asset_buffer)) {
        ar_log(AR_LOG_FATAL, "Asset buffer not initialized!\n", 0);
        exit(1);
    }
    asset_thread_state = AR_ASSET_THREAD_WAIT;

    ar_asset_t *asset = ar_buffer_new_item(&asset_buffer);
    asset->type = type;
    asset->status = AR_ASSET_QUEUED;  
    asset->data.raw = NULL;
    if (asset->type == AR_ASSET_MESH)
        asset->data.mesh = ar_mesh_new();

    ar_asset_queue_item_t *item = ar_buffer_new_item(&asset_queue);
    item->operation = AR_ASSET_OP_LOAD;
    item->path = path;
    item->asset = asset;

    asset_thread_state = AR_ASSET_THREAD_RUNNING;

    return asset;
}

ar_mesh_t *ar_asset_get_mesh(ar_asset_t *asset) {
    if (asset->type != AR_ASSET_MESH)
        return NULL;

    //if (asset->status == AR_ASSET_IN_MEM)
    //    ar_asset_resolve(asset);

    return asset->data.mesh;
}

void ar_asset_check_queue(void) {
    unsigned i;
    ar_asset_queue_item_t *item;
    for (i = 0; i < asset_queue.index; i++) {
        item = ar_buffer_get(&asset_queue, i);

        if (item->asset->status != AR_ASSET_QUEUED)
            continue;

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
    while (asset->status == AR_ASSET_QUEUED) {
        ar_thread_sleep(AR_ASSET_SLEEP_TIME);
    }
}

void ar_asset_destroy(ar_asset_t *asset) {
    ar_asset_queue_item_t *item = ar_buffer_new_item(&asset_queue);
    item->operation = AR_ASSET_OP_FREE;
    item->path = NULL;
    item->asset = asset;
}

void ar_asset_resolve(ar_asset_t *asset) {
    if (asset->status != AR_ASSET_IN_MEM) {
        ar_log(AR_LOG_WARN, "Attempting to resolve unloaded asset\n", 0);
        return;
    }
    switch (asset->type) {
        case AR_ASSET_MESH:
            ar_mesh_init(asset->data.mesh, 0);
            break;
        default:
            break;
    }
    asset->status = AR_ASSET_LOADED;
}

static void asset_load(ar_asset_queue_item_t *item) {
    switch (item->asset->type) {
        case AR_ASSET_MESH:
            ar_log(AR_LOG_INFO, "Load asset %s\n", item->path);
            item->asset->data.mesh = ar_mesh_load_raw(item->asset->data.mesh, item->path, MODEL_OBJ, 0);
            break;
        default:
            break;
    }
    item->asset->status = AR_ASSET_IN_MEM;
    (void)item;
}

static void asset_destroy(ar_asset_queue_item_t *item) {
    switch (item->asset->type) {
        case AR_ASSET_MESH:
            ar_mesh_destroy(item->asset->data.mesh);
            break;
        default:
            break;
    }
    ar_buffer_item_free(&asset_buffer, ar_buffer_get_item_index(&asset_buffer, item->asset));
}
