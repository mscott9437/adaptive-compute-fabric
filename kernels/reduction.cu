#include "reduction.hpp"

#include <vector>

#include <cuda_runtime.h>

#include "../include/cuda_check.hpp"

__global__
void reduction_kernel(
    const float* input,
    float* output,
    int size
)
{
    extern __shared__ float sdata[];

    unsigned int tid =
    threadIdx.x;

    unsigned int idx =
    blockIdx.x *
    blockDim.x +
    threadIdx.x;

    sdata[tid] =
    (idx < size)
    ? input[idx]
    : 0.0f;

    __syncthreads();

    for (
        unsigned int stride =
        blockDim.x / 2;
    stride > 0;
    stride >>= 1
    )
    {
        if (tid < stride)
        {
            sdata[tid] +=
            sdata[
                tid + stride
            ];
        }

        __syncthreads();
    }

    if (tid == 0)
    {
        output[blockIdx.x] =
        sdata[0];
    }
}

float launch_reduction(
    const float* device_input,
    int size,
    int block_size
)
{
    int threads =
        block_size;

    int blocks =
    (size + threads - 1)
    / threads;

    float* device_partial =
    nullptr;

    CUDA_CHECK(
        cudaMalloc(
            &device_partial,
            blocks *
            sizeof(float)
        )
    );

    reduction_kernel<<<
    blocks,
    threads,
    threads *
    sizeof(float)
    >>>(
        device_input,
        device_partial,
        size
    );

    CUDA_CHECK(
        cudaGetLastError()
    );

    std::vector<float> partial(
        blocks
    );

    CUDA_CHECK(
        cudaMemcpy(
            partial.data(),
                   device_partial,
                   blocks *
                   sizeof(float),
                   cudaMemcpyDeviceToHost
        )
    );

    float result = 0.0f;

    for (
        const auto& value :
        partial
    )
    {
        result += value;
    }

    CUDA_CHECK(
        cudaFree(
            device_partial
        )
    );

    return result;
}
