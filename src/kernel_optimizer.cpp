#include "../include/kernel_optimizer.hpp"

KernelOptimization
KernelOptimizer::analyze(
    const KernelState& state,
    const KernelReport& report
)
{
    KernelOptimization opt;

    //
    // Memory pressure
    //

    if (
        state.memory_pressure > 70.0f
    )
    {
        opt.recommendations.push_back(
            "Improve memory coalescing."
        );

        opt.recommendations.push_back(
            "Reduce global memory accesses."
        );

        opt.recommendations.push_back(
            "Increase data locality."
        );
    }

    //
    // Execution pressure
    //

    if (
        state.execution_pressure > 70.0f
    )
    {
        opt.recommendations.push_back(
            "Increase instruction level parallelism."
        );

        opt.recommendations.push_back(
            "Reduce dependency chains."
        );

        opt.recommendations.push_back(
            "Unroll critical loops where beneficial."
        );
    }

    //
    // Scheduler pressure
    //

    if (
        state.scheduler_pressure > 70.0f
    )
    {
        opt.recommendations.push_back(
            "Increase eligible warps."
        );

        opt.recommendations.push_back(
            "Reduce long-latency dependencies."
        );

        opt.recommendations.push_back(
            "Improve instruction scheduling."
        );
    }

    //
    // Synchronization pressure
    //

    if (
        state.synchronization_pressure > 20.0f
    )
    {
        opt.recommendations.push_back(
            "Reduce block synchronization."
        );

        opt.recommendations.push_back(
            "Minimize __syncthreads() usage."
        );

        opt.recommendations.push_back(
            "Reduce serialization between warps."
        );
    }

    if(state.execution_pipe_bound)
    {
        opt.recommendations.push_back(
            "Reduce execution pipeline contention."
        );

        opt.recommendations.push_back(
            "Distribute arithmetic operations."
        );
    }

    if(state.memory_pipeline_bound)
    {
        opt.recommendations.push_back(
            "Reduce memory pipeline pressure."
        );

        opt.recommendations.push_back(
            "Overlap memory operations."
        );
    }

    if(state.texture_pipeline_bound)
    {
        opt.recommendations.push_back(
            "Reduce texture fetch pressure."
        );
    }

    if(state.cache_bound)
    {
        opt.recommendations.push_back(
            "Improve cache locality."
        );

        opt.recommendations.push_back(
            "Increase data reuse."
        );
    }

    if(state.cache_thrashing)
    {
        opt.recommendations.push_back(
            "Increase spatial locality."
        );

        opt.recommendations.push_back(
            "Improve cache reuse."
        );
    }

    if(state.streaming_memory)
    {
        opt.recommendations.push_back(
            "Use vectorized memory loads."
        );
    }

    return opt;
}
