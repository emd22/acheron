#include <acheron/engine/renderer/rr_framebuffer.h>


ar_framebuffer_t ar_shadows_dp_new(int block_w, int block_h) {
    ar_framebuffer_t fb = ar_framebuffer_new(block_w*2, block_h);

    return fb;
}