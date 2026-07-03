#pragma once

#include <cuda_runtime.h>

__global__
void vector_add_kernel(
    const float* a,
    const float* b,
    float* c,
    int size
);

void launch_vector_add(
    const float* a,
    const float* b,
    float* c,
    int size,
    int block_size
);
