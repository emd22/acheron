#ifndef AR_MT_VECTOR_ARITH_H
#define AR_MT_VECTOR_ARITH_H

#include <f3d/engine/math/vector/mt_vector_math.h>

void ar_vector_add(ar_vector_type_t type, void *vec0, void *vec1, void *result);
void ar_vector_sub(ar_vector_type_t type, void *vec0, void *vec1, void *result);
void ar_vector_mul(ar_vector_type_t type, void *vec0, void *vec1, void *result);
void ar_vector_div(ar_vector_type_t type, void *vec0, void *vec1, void *result);

#endif
