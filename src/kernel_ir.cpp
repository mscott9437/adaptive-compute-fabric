#include "../include/kernel_ir.hpp"

KernelIR
KernelIRBuilder::build(
    const KernelReport& report,
    const KernelState& state,
    const KernelClassification& classification,
    const KernelOptimization& optimization
)
{
    KernelIR ir;

    ir.kernel = report.kernel;

    ir.block_size = report.tested_block_size;

    ir.runtime_ms = report.runtime_ms;

    ir.occupancy = report.occupancy;

    ir.throughput = report.sm_throughput_pct;

    ir.state = state;

    ir.classification = classification;

    ir.optimization = optimization;

    if(state.memory_bound)
        ir.bottleneck = "Memory";

    else if(state.execution_bound)
        ir.bottleneck = "Execution";

    else if(state.scheduler_bound)
        ir.bottleneck = "Scheduler";

    else if(state.synchronization_bound)
        ir.bottleneck = "Synchronization";

    else
        ir.bottleneck = "Mixed";

    if(state.memory_ratio > 0.5f)
        ir.dominant_instruction = "Memory";

    else if(state.fp32_ratio > 0.5f)
        ir.dominant_instruction = "FP32";

    else if(state.integer_ratio > 0.5f)
        ir.dominant_instruction = "Integer";

    else
        ir.dominant_instruction = "Mixed";

    if(state.execution_pipe_bound)
        ir.dominant_pipeline = "Execution";

    else if(state.memory_pipeline_bound)
        ir.dominant_pipeline = "Memory";

    else if(state.texture_pipeline_bound)
        ir.dominant_pipeline = "Texture";

    else
        ir.dominant_pipeline = "Balanced";

    ir.cache_hit_ratio =
    state.cache_hit_ratio;

    ir.streaming_memory =
    state.streaming_memory;

    ir.cache_friendly =
    state.cache_friendly;

    ir.cache_thrashing =
    state.cache_thrashing;

    ir.random_access =
    state.random_access;

    ir.execution_pipe_pressure =
    state.execution_pipe_pressure;

    ir.memory_pipeline_pressure =
    state.memory_pipeline_pressure;

    ir.texture_pipeline_pressure =
    state.texture_pipeline_pressure;
    return ir;
}
