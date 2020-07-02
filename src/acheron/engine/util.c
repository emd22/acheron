#include <acheron/engine/util.h>

// djb2 hashing function
// http://www.cse.yorku.ca/~oz/hash.html
hash_t util_hash_str(const char *str) {
    hash_t hash = 5381;
    int ch;
    while ((ch = (unsigned char)(*str++)))
        hash = ((hash << 5)+hash)+ch;
        
    return hash;
}
