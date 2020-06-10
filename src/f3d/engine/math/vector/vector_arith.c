#include <f3d/engine/math/vector/vector.h>

void ar_vector_add(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value += ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}

void ar_vector_sub(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value -= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}

void ar_vector_mul(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value *= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}

void ar_vector_div(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value /= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}
