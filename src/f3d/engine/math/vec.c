#include <f3d/engine/type/vec.h>
#include <f3d/engine/core/log.h>

#include <stddef.h> // offsetof

void vecf_set_at(int size, void *vector, int index, float value) {
    if (index > size)
        return;
    // NOTE: normally we wouldn't access a struct as an array, but because a float is 4 bytes,
    // alignment shouldn't be an issue.
    float *data = (float *)((float *)vector);
    data[index] = value;
}

float vecf_get_at(int size, void *vector, int index) {
    if (index > size)
        return 0.0f;
    float *data = (float *)((float *)vector);
    return data[index];
}

