#include "vector_add.hpp"

#include <cuda_runtime.h>

#include "../include/cuda_check.hpp"

__global__
void vector_add_kernel(
    const float* a,
    const float* b,
    float* c,
    int size
)
{
    int idx =
        blockIdx.x * blockDim.x
        + threadIdx.x;

    if (idx < size)
    {
        c[idx] =
            a[idx]
            + b[idx];
    }
}

void launch_vector_add(
    const float* a,
    const float* b,
    float* c,
    int size,
    int block_size
)
{
    int blocks =
        (
            size
            + block_size
            - 1
        )
        /
        block_size;

    vector_add_kernel<<<
        blocks,
        block_size
    >>>(
        a,
        b,
        c,
        size
    );

    CUDA_CHECK(
        cudaGetLastError()
    );

    CUDA_CHECK(
        cudaDeviceSynchronize()
    );
}
