#include "../include/kernel_classifier.hpp"

#include <algorithm>

KernelClassification
KernelClassifier::classify(
    const KernelReport& report
)
{
    KernelClassification result;

    //
    // Memory Bound
    //
    if (
        report.stall_long_scoreboard_pct > 40.0 &&
        report.warps_eligible_pct < 10.0
    )
    {
        result.label =
            "Memory Bound";

        result.confidence =
            std::min(
                100.0,
                report.stall_long_scoreboard_pct +
                (10.0 - report.warps_eligible_pct)
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

        result.evidence.push_back(
            "Warps Active: "
            + std::to_string(
                report.warps_active_pct
            ) + "%"
        );

        result.evidence.push_back(
            "Occupancy: "
            + std::to_string(
                report.occupancy * 100.0
            ) + "%"
        );

        return result;
    }

    //
    // Synchronization Bound
    //
    if (
        report.stall_barrier_pct > 20.0
    )
    {
        result.label =
            "Synchronization Bound";

        result.confidence =
            std::min(
                100.0,
                report.stall_barrier_pct * 3.0
            );

        result.evidence.push_back(
            "Barrier Stall: "
            + std::to_string(
                report.stall_barrier_pct
            ) + "%"
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
    // Compute Bound
    //
    if (
        report.sm_throughput_pct > 75.0 &&
        report.stall_long_scoreboard_pct < 20.0 &&
        report.stall_barrier_pct < 10.0
    )
    {
        result.label =
            "Compute Bound";

        result.confidence =
            std::min(
                100.0,
                report.sm_throughput_pct
            );

        result.evidence.push_back(
            "SM Throughput: "
            + std::to_string(
                report.sm_throughput_pct
            ) + "%"
        );

        result.evidence.push_back(
            "Warps Active: "
            + std::to_string(
                report.warps_active_pct
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
