#include "../include/nsight_capture.hpp"

#include "../kernels/vector_add.hpp"
#include "../kernels/reduction.hpp"

#include <vector>

#include <cuda_runtime.h>

void run_profile_vector_add(
    int block_size
)
{
    constexpr int elements =
        1'000'000;

    std::vector<float> host_a(
        elements,
        1.0f
    );

    std::vector<float> host_b(
        elements,
        2.0f
    );

    float* device_a = nullptr;
    float* device_b = nullptr;
    float* device_c = nullptr;

    cudaMalloc(
        &device_a,
        elements * sizeof(float)
    );

    cudaMalloc(
        &device_b,
        elements * sizeof(float)
    );

    cudaMalloc(
        &device_c,
        elements * sizeof(float)
    );

    cudaMemcpy(
        device_a,
        host_a.data(),
        elements * sizeof(float),
        cudaMemcpyHostToDevice
    );

    cudaMemcpy(
        device_b,
        host_b.data(),
        elements * sizeof(float),
        cudaMemcpyHostToDevice
    );

    launch_vector_add(
        device_a,
        device_b,
        device_c,
        elements,
        block_size
    );

    cudaDeviceSynchronize();

    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_c);
}

void run_profile_reduction(
    int block_size
)
{
    constexpr int elements =
        1'000'000;

    std::vector<float> host_input(
        elements,
        1.0f
    );

    float* device_input =
        nullptr;

    cudaMalloc(
        &device_input,
        elements * sizeof(float)
    );

    cudaMemcpy(
        device_input,
        host_input.data(),
        elements * sizeof(float),
        cudaMemcpyHostToDevice
    );

    launch_reduction(
        device_input,
        elements,
        block_size
    );

    cudaDeviceSynchronize();

    cudaFree(
        device_input
    );
}
