#include <algorithm>

#include "../include/kernel_state.hpp"

KernelState
KernelStateBuilder::build(
    const KernelReport& report
)
{
    KernelState state;

    //
    // Pressure scores
    //

    state.memory_pressure =
    std::clamp(
        static_cast<float>(
            report.stall_long_scoreboard_pct
        ),
        0.0f,
        100.0f
    );

    state.scheduler_pressure =
    std::clamp(
        100.0f -
        static_cast<float>(
            report.warps_eligible_pct
        ),
        0.0f,
        100.0f
    );

    state.execution_pressure =
    std::clamp(
        100.0f -
        static_cast<float>(
            report.inst_executed_per_cycle_active_pct
        ),
        0.0f,
        100.0f
    );

    state.synchronization_pressure =
    std::clamp(
        static_cast<float>(
            report.stall_barrier_pct
        ),
        0.0f,
        100.0f
    );

    state.utilization_score =
    std::clamp(
        static_cast<float>(
            report.sm_throughput_pct
        ),
        0.0f,
        100.0f
    );

    state.execution_pipe_pressure =
    std::clamp(
        static_cast<float>(
            report.stall_math_pipe_pct
        ),
        0.0f,
        100.0f
    );

    state.memory_pipeline_pressure =
    std::clamp(
        static_cast<float>(
            report.stall_mio_pct
        ),
        0.0f,
        100.0f
    );

    state.texture_pipeline_pressure =
    std::clamp(
        static_cast<float>(
            report.stall_tex_pipe_pct
        ),
        0.0f,
        100.0f
    );

    state.cache_pressure =
    100.0f -
    report.l1_hit_rate_pct;

    //
    // Bottleneck flags
    //

    state.memory_bound =
    state.memory_pressure > 50.0f;

    state.execution_bound =
    state.execution_pressure > 50.0f;

    state.scheduler_bound =
    state.scheduler_pressure > 70.0f;

    state.synchronization_bound =
    state.synchronization_pressure > 20.0f;

    state.execution_pipe_bound =
    state.execution_pipe_pressure > 40.0f;

    state.memory_pipeline_bound =
    state.memory_pipeline_pressure > 40.0f;

    state.texture_pipeline_bound =
    state.texture_pipeline_pressure > 30.0f;

    state.cache_bound =
    state.cache_pressure > 40.0f;

    float total =
    report.l2_hits +
    report.l2_misses;

    if(total > 0.0f)
    {
        state.cache_hit_ratio =
        report.l2_hits / total;
    }

    state.cache_pressure =
    100.0f *
    (1.0f - state.cache_hit_ratio);

    state.cache_bound =
    state.cache_pressure > 40.0f;

    state.streaming_memory =
    report.l1_global_load_requests >
    report.l2_hits;

    state.cache_friendly =
    state.cache_hit_ratio > 0.80f;

    state.cache_thrashing =
    state.cache_pressure > 60.0f;

    state.random_access =
    report.l2_misses >
    report.l2_hits;

    float total_inst =
    report.inst_fp32 +
    report.inst_fp64 +
    report.inst_integer +
    report.inst_memory;

    if(total_inst > 0.0f)
    {
        state.fp32_ratio =
        report.inst_fp32 /
        total_inst;

        state.fp64_ratio =
        report.inst_fp64 /
        total_inst;

        state.integer_ratio =
        report.inst_integer /
        total_inst;

        state.memory_ratio =
        report.inst_memory /
        total_inst;
    }

    state.l1_hit_rate =
    report.l1_hit_rate_pct;

    state.l2_hit_rate =
    report.l2_hit_rate_pct;

    return state;
}
