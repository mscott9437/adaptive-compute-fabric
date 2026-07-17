#include "../include/kernel_classifier.hpp"

#include <algorithm>

KernelClassification
KernelClassifier::classify(
    const KernelState& state,
    const KernelReport& report
)
{
    KernelClassification result;

    //
    // Memory Bound
    //
    if(state.memory_bound)
    {
        result.label =
        "Memory Bound";

        result.confidence =
        std::min(
            100.0,
            static_cast<double>(
                state.memory_pressure
            )
        );

        result.evidence.push_back(
            "Memory Pressure: "
            + std::to_string(
                state.memory_pressure
            ) + "%"
        );

        result.evidence.push_back(
            "Long Scoreboard Stall: "
            + std::to_string(
                report.stall_long_scoreboard_pct
            ) + "%"
        );

        result.evidence.push_back(
            "Warps Eligible: "
            + std::to_string(
                report.warps_eligible_pct
            ) + "%"
        );

        return result;
    }

    //
    // Execution Bound
    //
    if(state.execution_bound)
    {
        result.label =
        "Execution Bound";

            result.confidence =
            std::min(
                100.0,
                static_cast<double>(
                    state.execution_pressure
                )
            );

            result.evidence.push_back(
                "Execution Pressure: "
                + std::to_string(
                    state.execution_pressure
                ) + "%"
            );

            result.evidence.push_back(
                "Instructions/Cycle: "
                + std::to_string(
                    report.inst_executed_per_cycle_active_pct
                )
            );

            return result;
    }

    //
    // Scheduler Bound
    //
    if(state.scheduler_bound)
    {
        result.label =
        "Scheduler Bound";

                result.confidence =
                std::min(
                    100.0,
                    static_cast<double>(
                        state.scheduler_pressure
                    )
                );

                result.evidence.push_back(
                    "Scheduler Pressure: "
                    + std::to_string(
                        state.scheduler_pressure
                    ) + "%"
                );

                result.evidence.push_back(
                    "Warps Eligible: "
                    + std::to_string(
                        report.warps_eligible_pct
                    ) + "%"
                );

                return result;
    }

    //
    // Synchronization Bound
    //
    if(state.synchronization_bound)
    {
        result.label =
        "Synchronization Bound";

                    result.confidence =
                    std::min(
                        100.0,
                        static_cast<double>(
                            state.synchronization_pressure
                        )
                    );

                    result.evidence.push_back(
                        "Synchronization Pressure: "
                        + std::to_string(
                            state.synchronization_pressure
                        ) + "%"
                    );

                    result.evidence.push_back(
                        "Barrier Stall: "
                        + std::to_string(
                            report.stall_barrier_pct
                        ) + "%"
                    );

                    return result;
    }

    //
    // Execution Pipe Bound
    //
    if(state.execution_pipe_bound)
    {
        result.label =
        "Execution Pipeline Bound";

                        result.confidence =
                        std::min(
                            100.0,
                            static_cast<double>(
                                state.execution_pipe_pressure
                            )
                        );

                        result.evidence.push_back(
                            "Math Pipe Stall: "
                            + std::to_string(
                                report.stall_math_pipe_pct
                            ) + "%"
                        );

                        result.evidence.push_back(
                            "Execution Pipe Pressure: "
                            + std::to_string(
                                state.execution_pipe_pressure
                            ) + "%"
                        );

                        return result;
    }

    //
    // Memory Pipeline Bound
    //
    if(state.memory_pipeline_bound)
    {
        result.label =
        "Memory Pipeline Bound";

                        result.confidence =
                        std::min(
                            100.0,
                            static_cast<double>(
                                state.memory_pipeline_pressure
                            )
                        );

                        result.evidence.push_back(
                            "MIO Stall: "
                            + std::to_string(
                                report.stall_mio_pct
                            ) + "%"
                        );

                        return result;
    }

    //
    // Texture Pipeline Bound
    //
    if(state.texture_pipeline_bound)
    {
        result.label =
        "Texture Pipeline Bound";

        result.confidence =
        std::min(
            100.0,
            static_cast<double>(
                state.texture_pipeline_pressure
            )
        );

        result.evidence.push_back(
            "Texture Stall: "
            + std::to_string(
                report.stall_tex_pipe_pct
            ) + "%"
        );

        return result;
    }

    //
    // Cache Bound
    //
    if(state.cache_bound)
    {
        result.label =
        "Cache Bound";

            result.confidence =
            std::min(
                100.0,
                static_cast<double>(
                    state.cache_pressure
                )
            );

            result.evidence.push_back(
                "L1 Hit Rate: "
                + std::to_string(
                    report.l1_hit_rate_pct
                ) + "%"
            );

            result.evidence.push_back(
                "L2 Hit Rate: "
                + std::to_string(
                    report.l2_hit_rate_pct
                ) + "%"
            );

            return result;
    }

    if(state.cache_friendly)
    {
        result.evidence.push_back(
            "High cache hit ratio."
        );
    }

    if(state.streaming_memory)
    {
        result.evidence.push_back(
            "Streaming memory access pattern."
        );
    }

    if(state.cache_thrashing)
    {
        result.label = "Cache Thrashing";
    }

    if(state.random_access)
    {
        result.evidence.push_back(
            "Random memory access detected."
        );
    }

    //
    // FP32 Compute
    //

    if(state.fp32_ratio > 0.60f)
    {
        result.evidence.push_back(
            "FP32 Dominated Kernel"
        );
    }

    if(state.integer_ratio > 0.60f)
    {
        result.evidence.push_back(
            "Integer Dominated Kernel"
        );
    }

    if(state.memory_ratio > 0.60f)
    {
        result.evidence.push_back(
            "Memory Instruction Dominated"
        );
    }

    if(state.l1_hit_rate < 50.0f)
    {
        result.evidence.push_back(
            "Poor L1 locality"
        );
    }

    if(state.l2_hit_rate < 60.0f)
    {
        result.evidence.push_back(
            "Poor L2 locality"
        );
    }

    //
    // Compute Bound
    //
    if(
        report.sm_throughput_pct > 75.0 &&
        !state.memory_bound &&
        !state.execution_bound &&
        !state.scheduler_bound &&
        !state.synchronization_bound &&
        !state.execution_pipe_bound &&
        !state.memory_pipeline_bound &&
        !state.texture_pipeline_bound &&
        !state.cache_bound
    )
    {
        result.label =
        "Compute Bound";

                result.confidence =
                std::min(
                    100.0,
                    static_cast<double>(
                        report.sm_throughput_pct
                    )
                );

                result.evidence.push_back(
                    "SM Throughput: "
                    + std::to_string(
                        report.sm_throughput_pct
                    ) + "%"
                );

                return result;
    }

    //
    // Fallback
    //
    result.label =
        "Mixed Workload";

    result.confidence =
        50.0;

    return result;
}
