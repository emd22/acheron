#include <acheron/engine/math/vector/mt_vector_arith.h>
#include <acheron/engine/math/vector/mt_vector_math.h>
#include <acheron/engine/core/cr_log.h>

void ar_vector_add(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = _AR_VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value += ar_vectorf_get(type, vec1, i);   
        ar_vectorf_set(type, result, i, value);
    }
}

void ar_vector_sub(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = _AR_VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value -= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}

void *ar_vector_mul_value(ar_vector_type_t type, void *vec, float value, void *result) {
    const int length = _AR_VEC_LENGTH(type);
    int i;
    float res;
    for (i = 0; i < length; i++) {
        res = ar_vectorf_get(type, vec, i);
        res *= value;    
        ar_vectorf_set(type, result, i, res);
    }
    return result;
}

void *ar_vector_mul(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = _AR_VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value *= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
    return result;
}

void ar_vector_div(ar_vector_type_t type, void *vec0, void *vec1, void *result) {
    const int length = _AR_VEC_LENGTH(type);
    int i;
    float value;
    for (i = 0; i < length; i++) {
        value = ar_vectorf_get(type, vec0, i);
        value /= ar_vectorf_get(type, vec1, i);    
        ar_vectorf_set(type, result, i, value);
    }
}
