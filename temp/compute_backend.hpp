#pragma once

#include <string>

struct BenchmarkResult
{
    std::string backend;

    double runtime_ms = 0.0;

    double occupancy = 0.0;

    double l2_hit_rate = 0.0;

    double warp_divergence = 0.0;
};

class ComputeBackend
{
public:
    virtual ~ComputeBackend() = default;

    virtual BenchmarkResult
    vector_add(
        float* a,
        float* b,
        float* c,
        int size
    ) = 0;

    virtual const char*
    name() const = 0;
};
