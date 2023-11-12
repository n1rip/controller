#pragma once

/**
 * @brief 3-dimensional vector
 */
typedef struct n1_vec3_s {
    float x;
    float y;
    float z;
} n1_vec3_t;

float get_vector_length(n1_vec3_t* vec);
