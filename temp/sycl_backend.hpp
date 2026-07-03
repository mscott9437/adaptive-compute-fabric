#pragma once

#include "../../include/compute_backend.hpp"

class SyclBackend
    : public ComputeBackend
{
public:

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
        return "sycl";
    }
};
