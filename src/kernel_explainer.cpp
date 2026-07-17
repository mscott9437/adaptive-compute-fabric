#include "../include/kernel_explainer.hpp"

#include <sstream>
#include <iomanip>

KernelExplanation
KernelExplainer::explain(
    const KernelReport& winner,
    const KernelReport& runner_up
)
{
    KernelExplanation explanation;

    std::ostringstream ss;

    auto add_runtime_improvement =
        [&](void)
        {
            double delta_ms =
                runner_up.runtime_ms -
                winner.runtime_ms;

            double delta_pct =
                (delta_ms / runner_up.runtime_ms) * 100.0;

            ss.str("");
            ss.clear();

            ss << std::fixed
               << std::setprecision(3);

            ss
                << "Runtime improved by "
                << delta_ms * 1000.0
                << " us ("
                << delta_pct
                << "%)";

            explanation.evidence.push_back(
                ss.str()
            );
        };

    //
    // Memory Bound
    //
    if (winner.kernel_classification == "Memory Bound")
    {
        explanation.summary =
            "The winning launch configuration achieved the fastest execution while exhibiting the scheduling characteristics of a memory-bound kernel.";

        //
        // Warps Eligible
        //
        ss.str("");
        ss.clear();

        if (winner.warps_eligible_pct >
            runner_up.warps_eligible_pct)
        {
            ss
                << "Warps Eligible increased from "
                << runner_up.warps_eligible_pct
                << "% to "
                << winner.warps_eligible_pct
                << "%";
        }
        else
        {
            ss
                << "Warps Eligible decreased from "
                << runner_up.warps_eligible_pct
                << "% to "
                << winner.warps_eligible_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        //
        // Long Scoreboard Stall
        //
        ss.str("");
        ss.clear();

        if (winner.stall_long_scoreboard_pct <
            runner_up.stall_long_scoreboard_pct)
        {
            ss
                << "Long Scoreboard Stall decreased from "
                << runner_up.stall_long_scoreboard_pct
                << "% to "
                << winner.stall_long_scoreboard_pct
                << "%";
        }
        else
        {
            ss
                << "Long Scoreboard Stall increased from "
                << runner_up.stall_long_scoreboard_pct
                << "% to "
                << winner.stall_long_scoreboard_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        //
        // Warps Active
        //
        ss.str("");
        ss.clear();

        if (winner.warps_active_pct >
            runner_up.warps_active_pct)
        {
            ss
                << "Warps Active increased from "
                << runner_up.warps_active_pct
                << "% to "
                << winner.warps_active_pct
                << "%";
        }
        else
        {
            ss
                << "Warps Active decreased from "
                << runner_up.warps_active_pct
                << "% to "
                << winner.warps_active_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        add_runtime_improvement();

        return explanation;
    }

    //
    // Synchronization Bound
    //
    if (winner.kernel_classification == "Synchronization Bound")
    {
        explanation.summary =
            "The winning launch configuration maintained high SM utilization while reducing synchronization overhead.";

        //
        // Barrier Stall
        //
        ss.str("");
        ss.clear();

        if (winner.stall_barrier_pct <
            runner_up.stall_barrier_pct)
        {
            ss
                << "Barrier Stall decreased from "
                << runner_up.stall_barrier_pct
                << "% to "
                << winner.stall_barrier_pct
                << "%";
        }
        else
        {
            ss
                << "Barrier Stall increased from "
                << runner_up.stall_barrier_pct
                << "% to "
                << winner.stall_barrier_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        //
        // SM Throughput
        //
        ss.str("");
        ss.clear();

        if (winner.sm_throughput_pct >
            runner_up.sm_throughput_pct)
        {
            ss
                << "SM Throughput increased from "
                << runner_up.sm_throughput_pct
                << "% to "
                << winner.sm_throughput_pct
                << "%";
        }
        else
        {
            ss
                << "SM Throughput decreased from "
                << runner_up.sm_throughput_pct
                << "% to "
                << winner.sm_throughput_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        add_runtime_improvement();

        return explanation;
    }

    //
    // Compute Bound
    //
    if (winner.kernel_classification == "Compute Bound")
    {
        explanation.summary =
            "The winning launch configuration achieved better execution throughput across the streaming multiprocessors.";

        ss.str("");
        ss.clear();

        if (winner.sm_throughput_pct >
            runner_up.sm_throughput_pct)
        {
            ss
                << "SM Throughput increased from "
                << runner_up.sm_throughput_pct
                << "% to "
                << winner.sm_throughput_pct
                << "%";
        }
        else
        {
            ss
                << "SM Throughput decreased from "
                << runner_up.sm_throughput_pct
                << "% to "
                << winner.sm_throughput_pct
                << "%";
        }

        explanation.evidence.push_back(
            ss.str()
        );

        add_runtime_improvement();

        return explanation;
    }

    //
    // Default
    //
    explanation.summary =
        "The winning launch configuration produced the fastest execution while maintaining similar scheduler behavior.";

    add_runtime_improvement();

    return explanation;
}
