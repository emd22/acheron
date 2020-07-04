#include <acheron/engine/asset/at_assetman.h>
#include <acheron/engine/limits.h>

#include <string.h>

#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/threads/cr_threads.h>

static ar_thread_t asset_thread;
static char asset_folder[AR_PATH_LENGTH];

typedef struct {
    char filename[AR_PATH_LENGTH];
    ar_asset_type_t asset_type;
    ar_asset_status_t status;
    ar_asset_t asset;
} ar_asset_queue_item_t;

static ar_buffer_t asset_queue;

static void load_asset(ar_asset_queue_item_t *item) {
    if (item->type == AR_ASSET_IMAGE) {
        
    }
}

void *ar_assetman_check_queue(void *arg) {
    (void)arg;
    int i;
    ar_asset_queue_item_t *item;
    for (i = 0; i < AR_ASSET_QUEUE_SIZE; i++) {
        item = ar_buffer_get(&asset_queue, i);
        if (item->status == AR_ASSET_LOAD) {
            
        }
    }
    ar_thread_sleep(500);
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

void ar_assets_set_path(const char *folder_path) {
    strcpy(asset_folder, folder_path);
}
