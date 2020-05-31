#include <f3d/engine/acheron.h>

int main() {
    ar_instance_t *instance = ar_instance_new(AR_INSTANCE_GRAPHICS);
    ar_window_t *window = ar_window_new("Acheron3d", 700, 700, 0);
    ar_instance_attach(instance, AR_INSTANCE_ATTACH_WINDOW, window);
    
    while (instance->running) {
        ar_window_buffers_swap(window);
    }
    ar_window_destroy(window);
    ar_instance_destroy(instance);
}
