#pragma once

#include <cuda_runtime.h>

__global__
void reduction_kernel(
    const float* input,
    float* output,
    int size
);

float launch_reduction(
    const float* device_input,
    int size,
    int block_size
);
