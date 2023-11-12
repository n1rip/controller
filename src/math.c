#include <math.h>
#include "../include/math.h"

float get_vector_length(n1_vec3_t* vec) {
    return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}