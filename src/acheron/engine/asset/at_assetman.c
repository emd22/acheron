#include <acheron/engine/asset/at_assetman.h>
#include <acheron/engine/limits.h>

#include <string.h>
#include <stdlib.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/threads/cr_threadman.h>

typedef struct {
    char path[AR_PATH_LENGTH];
} ar_asset_type_info_t;

static ar_thread_t asset_thread;
static char asset_path[AR_PATH_LENGTH];

static ar_asset_type_info_t asset_type_info[AR_ASSET_TYPE_AMT];
static ar_buffer_t asset_queue;

static void load_asset(ar_asset_queue_item_t *item) {
    if (item->type == AR_ASSET_IMAGE) {
            
    }
}

static int asset_sort_func(const void *a, const void *b) {
    ar_asset_t *asset0 = (ar_asset_t *)a;
    ar_asset_t *asset1 = (ar_asset_t *)b;
    // sort from highest to lowest
    return (asset1->precedence-asset0->precedence);
}

static void *asset_thread(ar_thread_t *thread, void *arg) {
    (void)arg;
    int i;
    ar_asset_queue_item_t *item;
    while (thread->status == AR_THREAD_RUNNING) {
        for (i = 0; i < AR_ASSET_QUEUE_SIZE; i++) {
            item = ar_buffer_get(&asset_queue, i);
            if (item->status == AR_ASSET_LOAD)
        }
        
        ar_thread_sleep(300);    
    }
    thread->status = AR_THREAD_STOPPED;
    return NULL;
}

void *ar_assetman_check_queue(void *arg) {
    
    return NULL;
}

void ar_assetman_init(void) {
    asset_thread = ar_thread_new(ar_assetman_check_queue, NULL);
    ar_buffer_init(&asset_queue, AR_BUFFER_STATIC, sizeof(ar_asset_queue_item_t), AR_ASSET_QUEUE_SIZE, 0);
    int i;
    ar_asset_queue_item_t *item;
    for (i = 0; i < AR_ASSET_QUEUE_SIZE; i++) {
        item = ar_buffer_get(&asset_queue, i);
        item->status = AR_ASSET_NONE;
    }
}

ar_asset_t *ar_asset_load() {
    qsort(&asset_queue.data, asset_queue.index, sizeof(ar_asset_t), &asset_sort_func);
    ar_asset_t *asset = buffer_new_item(&asset_queue);
    asset->status = AR_ASSET_LOAD;
    return asset;
}

void ar_asset_destroy(ar_asset_t *asset) {
    asset->status = AR_ASSET_NONE;
    
    switch (asset->type) {
        case AR_ASSET_IMAGE:
        case AR_ASSET_MESH:
    }
    
}

void ar_assetman_set_path(const char *path) {
    strncpy();
}
