#pragma once

#include "../include/compute_backend.hpp"

//#include "cuda_device.hpp"
//#include "cuda_profiler.hpp"

class CudaBackend
    : public ComputeBackend
{
public:

    CudaBackend();

    BenchmarkResult
    vector_add(
        float* a,
        float* b,
        float* c,
        int size
    ) override;

    const char*
    name() const override
    {
        return "cuda";
    }
/*
private:

    CudaDevice device_;

    CudaProfiler profiler_;*/
};
