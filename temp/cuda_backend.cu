#include "cuda_backend.hpp"

#include "../kernels/vector_add.hpp"

#include <chrono>

CudaBackend::CudaBackend()
{
}

BenchmarkResult
CudaBackend::vector_add(
    float* a,
    float* b,
    float* c,
    int size
)
{
    auto start =
        std::chrono::high_resolution_clock::now();

    launch_vector_add(
        a,
        b,
        c,
        size,
        256
    );

    auto end =
        std::chrono::high_resolution_clock::now();

    BenchmarkResult result;

    result.backend = name();

    result.runtime_ms =
        std::chrono::duration<
            double,
            std::milli
        >(
            end - start
        ).count();

    return result;
}
