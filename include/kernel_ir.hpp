#pragma once

#include "kernel_report.hpp"
#include "kernel_state.hpp"
#include "kernel_classifier.hpp"
#include "kernel_optimizer.hpp"

struct KernelIR
{
    std::string kernel;

    KernelState state;

    KernelClassification classification;

    KernelOptimization optimization;

    int block_size;

    float runtime_ms;

    float occupancy;

    float throughput;

    std::vector<std::string> workflow_tags;

    std::string workflow;

    std::string dominant_instruction;

    std::string dominant_pipeline;

    std::string bottleneck;

    //
    // Cache analysis
    //

    float cache_hit_ratio = 0.0f;

    bool streaming_memory = false;

    bool cache_friendly = false;

    bool cache_thrashing = false;

    bool random_access = false;

    //
    // Pipeline analysis
    //

    float execution_pipe_pressure = 0.0f;

    float memory_pipeline_pressure = 0.0f;

    float texture_pipeline_pressure = 0.0f;
};

class KernelIRBuilder
{
public:

    static KernelIR build(
        const KernelReport&,
        const KernelState&,
        const KernelClassification&,
        const KernelOptimization&
    );
};
