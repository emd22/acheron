#include <acheron/engine/core/memory/mm_memory.h>
#include <acheron/engine/core/cr_debug.h>

#include <string.h>

/*
 * Safe strncpy implementation with null termination
 */
 
int ar_memory_strncpy(char *dest, char *src, int n) {
    ar_assert((dest && src));
    
    char *start = dest;
    do {
        /* break before n for null */
        if (dest-start == n-1)
            break;
        *(dest++) = *(src++);
    } while(*(src)); /* break if src byte is 0 */

    (*dest) = 0;
    return (dest-start == n-1); /* 1 if truncated */
}

