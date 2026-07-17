#pragma once

#include "kernel_report.hpp"

struct KernelState
{
    //
    // Bottleneck classification
    //

    bool memory_bound = false;

    bool execution_bound = false;

    bool synchronization_bound = false;

    bool scheduler_bound = false;

    bool cache_bound = false;

    bool instruction_bound = false;

    bool bandwidth_bound = false;

    bool latency_bound = false;

    //
    // Pipeline bottlenecks
    //

    bool execution_pipe_bound = false;

    bool memory_pipeline_bound = false;

    bool texture_pipeline_bound = false;

    //
    // Pressure scores
    //

    float scheduler_pressure = 0.0f;

    float execution_pressure = 0.0f;

    float memory_pressure = 0.0f;

    float cache_pressure = 0.0f;

    float synchronization_pressure = 0.0f;

    //
    // Pipeline pressures
    //

    float execution_pipe_pressure = 0.0f;

    float memory_pipeline_pressure = 0.0f;

    float texture_pipeline_pressure = 0.0f;

    //
    // Cache
    //

    float l1_hit_rate = 0.0f;
    float l2_hit_rate = 0.0f;

    float cache_hit_ratio = 0.0f;

    bool streaming_memory = false;

    bool cache_friendly = false;

    bool cache_thrashing = false;

    bool random_access = false;

    //
    // Instruction mix
    //

    float fp32_ratio = 0.0f;
    float fp64_ratio = 0.0f;
    float integer_ratio = 0.0f;
    float memory_ratio = 0.0f;

    //
    // Overall utilization
    //

    float utilization_score = 0.0f;

    //
    // Confidence

    float confidence = 0.0f;

    float math_pipe_pressure = 0.0f;

    float mio_pressure = 0.0f;

    float tex_pressure = 0.0f;

    float instruction_pressure = 0.0f;

    float bandwidth_pressure = 0.0f;

    bool math_pipe_bound = false;

    bool mio_bound = false;

    bool tex_bound = false;
};

class KernelStateBuilder
{
public:

    static KernelState build(
        const KernelReport& report
    );
};
