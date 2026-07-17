#include "../include/console_reporter.hpp"

#include <iostream>
#include <iomanip>

void ConsoleReporter::print(
    const KernelReport& report
)
{
    std::cout
        << "\n"
        << "==================================================\n"
        << report.kernel
        << "\n"
        << "==================================================\n\n";

    std::cout
        << "Backend: "
        << report.backend
        << "\n";

    std::cout
        << "Kernel: "
        << report.kernel
        << "\n\n";

    std::cout
        << "Result: "
        << (report.passed ? "PASS" : "FAIL")
        << "\n\n";

    std::cout
        << "Runtime\n"
        << "--------------------------------------------------\n";

    std::cout
        << "Runtime:                  "
        << report.runtime_ms
        << " ms\n";

    if (report.bandwidth_gb_s > 0.0)
    {
        std::cout
            << "Bandwidth:               "
            << report.bandwidth_gb_s
            << " GB/s\n";
    }

    if (report.throughput_gops > 0.0)
    {
        std::cout
            << "Throughput:              "
            << report.throughput_gops
            << " GOPS\n";
    }

    std::cout
        << "Efficiency Score:        "
        << report.efficiency_score
        << "\n\n";

    std::cout
        << "Workload\n"
        << "--------------------------------------------------\n";

    std::cout
        << "Elements:                "
        << report.elements
        << "\n";

    if (report.bytes_processed > 0)
    {
        std::cout
            << "Bytes Processed:         "
            << report.bytes_processed
            << "\n";
    }

    std::cout
        << "\nLaunch Configuration\n"
        << "--------------------------------------------------\n";

    std::cout
        << "Tested Block Size:       "
        << report.tested_block_size
        << "\n";

    std::cout
        << "Tested Grid Size:        "
        << report.tested_grid_size
        << "\n";

    std::cout
        << "Recommended Block Size:  "
        << report.recommended_block_size
        << "\n";

    std::cout
        << "Recommended Grid Size:   "
        << report.recommended_grid_size
        << "\n";

    std::cout
        << "\nOccupancy\n"
        << "--------------------------------------------------\n";

    std::cout
        << "Occupancy:               "
        << report.occupancy * 100.0f
        << " %\n";

    std::cout
        << "\nKernel Resources\n"
        << "--------------------------------------------------\n";

    std::cout
        << "Registers / Thread:      "
        << report.registers_per_thread
        << "\n";

    std::cout
        << "Shared Memory:           "
        << report.shared_memory_bytes
        << " bytes\n";

    std::cout
        << "Active Blocks / SM:      "
        << report.active_blocks_per_sm
        << "\n";

    std::cout
        << "Occupancy Raw:           "
        << report.occupancy
        << "\n";

    std::cout
        << "\nDevice\n"
        << "--------------------------------------------------\n";

    std::cout
        << "GPU:                     "
        << report.gpu_name
        << "\n";

    std::cout
        << "Compute Capability:      "
        << report.compute_capability
        << "\n";

    std::cout
        << "Global Memory:           "
        << report.global_memory_mb
        << " MB\n";

    std::cout << "\n";
}

void ConsoleReporter::print_nsight_summary(
    const KernelReport& report
)
{
    if (!report.has_nsight_metrics)
    {
        return;
    }

    std::cout
        << "\n========================================\n"
        << "NSIGHT ANALYSIS\n"
        << "========================================\n\n";

    std::cout
        << "Kernel: "
        << report.kernel
        << "\n";

    std::cout
        << "Winning Block Size: "
        << report.tested_block_size
        << "\n\n";

    std::cout
        << "DRAM Throughput: "
        << report.dram_throughput_pct
        << "%\n";

    std::cout
        << "Compute Memory Throughput: "
        << report.compute_memory_throughput_pct
        << "%\n";

    std::cout
        << "Nsight Kernel Time: "
        << report.nsight_kernel_time_ms
        << " ms\n";
        
	std::cout
    	<< "Max Warps Active: "
    	<< report.max_warps_active_pct
    	<< "%\n";
	
	std::cout
    	<< "SM Throughput: "
    	<< report.sm_throughput_pct
    	<< "%\n";

	std::cout
    	<< "Warps Active: "
    	<< report.warps_active_pct
    	<< "%\n";
    
	std::cout
    	<< "Warps Eligible: "
    	<< report.warps_eligible_pct
    	<< "%\n";
	
	std::cout
    	<< "Long Scoreboard Stall: "
    	<< report.stall_long_scoreboard_pct
    	<< "%\n";
	
	std::cout
    	<< "Short Scoreboard Stall: "
    	<< report.stall_short_scoreboard_pct
    	<< "%\n";
	
	std::cout
    	<< "Not Selected Stall: "
    	<< report.stall_not_selected_pct
    	<< "%\n";
	
	std::cout
    	<< "Barrier Stall: "
    	<< report.stall_barrier_pct
    	<< "%\n";

    std::cout
        << "Instructions/Cycle: "
        << report.inst_executed_per_cycle_active_pct
        << "%\n";

    std::cout
        << "Math Pipe Stall: "
        << report.stall_math_pipe_pct
        << "%\n";

    std::cout
        << "MIO Stall: "
        << report.stall_mio_pct
        << "%\n";

    std::cout
        << "Texture Stall: "
        << report.stall_tex_pipe_pct
        << "%\n";

    std::cout
        << "FP32 Instructions: "
        << report.inst_fp32
        << "\n";

    std::cout
        << "Integer Instructions: "
        << report.inst_integer
        << "\n";

    std::cout
        << "Memory Instructions: "
        << report.inst_memory
        << "\n";

    std::cout
        << "L1 Hit Rate: "
        << report.l1_hit_rate_pct
        << "%\n";

    std::cout
        << "L2 Hit Rate: "
        << report.l2_hit_rate_pct
        << "%\n";

	if (!report.kernel_classification.empty())
	{
    	std::cout
        	<< "\nKernel Classification\n"
        	<< "--------------------------------------------------\n";
	
    	std::cout
        	<< report.kernel_classification
        	<< "\n\n";
	
    	std::cout
        	<< "Confidence: "
        	<< report.classification_confidence
        	<< "%\n";
	}

	if (!report.classification_evidence.empty())
	{
    	std::cout
        	<< "\nEvidence:\n";
	
    	for (const auto& item :
         	report.classification_evidence)
    	{
        	std::cout
            	<< "- "
            	<< item
            	<< "\n";
    	}
	}
}

void ConsoleReporter::print_winner_comparison(
    const KernelReport& winner,
    const KernelReport& runner_up
)
{
    std::cout
        << "\n========================================\n"
        << "WINNER VS RUNNER-UP\n"
        << "========================================\n\n";

    std::cout
        << "Winner\n"
        << "----------------------------------------\n";

    std::cout
        << "Block Size: "
        << winner.tested_block_size
        << "\n";

    std::cout
        << "Classification: "
        << winner.kernel_classification
        << "\n\n";

    std::cout
        << "Runner-Up\n"
        << "----------------------------------------\n";

    std::cout
        << "Block Size: "
        << runner_up.tested_block_size
        << "\n";

    std::cout
        << "Classification: "
        << runner_up.kernel_classification
        << "\n\n";

    double delta_pct =
        (
            runner_up.runtime_ms
            - winner.runtime_ms
        )
        /
        winner.runtime_ms
        * 100.0;

    std::cout
        << "Runtime Difference: "
        << delta_pct
        << "%\n";
}

void ConsoleReporter::print_explanation(
    const KernelExplanation& explanation
)
{
    std::cout
        << "\n========================================\n"
        << "WHY THE WINNER WON\n"
        << "\n========================================\n\n";

    std::cout
        << explanation.summary
        << "\n\n";

    for (const auto& e : explanation.evidence)
    {
        std::cout
            << "- "
            << e
            << "\n";
    }
}

void ConsoleReporter::print_kernel_state(
    const KernelState& state
)
{
    std::cout
    << "\n========================================\n"
    << "KERNEL STATE\n"
    << "========================================\n\n";

    std::cout
    << "Memory Bound: "
    << (state.memory_bound ? "Yes" : "No")
    << "\n";

    std::cout
    << "Execution Bound: "
    << (state.execution_bound ? "Yes" : "No")
    << "\n";

    std::cout
    << "Scheduler Bound: "
    << (state.scheduler_bound ? "Yes" : "No")
    << "\n";

    std::cout
    << "Synchronization Bound: "
    << (state.synchronization_bound ? "Yes" : "No")
    << "\n\n";

    std::cout
    << "Memory Pressure: "
    << state.memory_pressure
    << "%\n";

    std::cout
    << "Execution Pressure: "
    << state.execution_pressure
    << "%\n";

    std::cout
    << "Scheduler Pressure: "
    << state.scheduler_pressure
    << "%\n";

    std::cout
    << "Utilization Score: "
    << state.utilization_score
    << "%\n";

    std::cout
    << "Execution Pipe Pressure: "
    << state.execution_pipe_pressure
    << "%\n";

    std::cout
    << "Memory Pipeline Pressure: "
    << state.memory_pipeline_pressure
    << "%\n";

    std::cout
    << "Texture Pipeline Pressure: "
    << state.texture_pipeline_pressure
    << "%\n";

    std::cout
    << "Cache Hit Ratio: "
    << state.cache_hit_ratio * 100.0f
    << "%\n";

    std::cout
    << "Streaming Memory: "
    << (state.streaming_memory ? "Yes" : "No")
    << "\n";

    std::cout
    << "Cache Friendly: "
    << (state.cache_friendly ? "Yes" : "No")
    << "\n";

    std::cout
    << "Cache Thrashing: "
    << (state.cache_thrashing ? "Yes" : "No")
    << "\n";

    std::cout
    << "Random Access: "
    << (state.random_access ? "Yes" : "No")
    << "\n";
}

void ConsoleReporter::print_classification(
    const KernelClassification& classification
)
{
    std::cout
    << "\n========================================\n"
    << "CLASSIFICATION\n"
    << "========================================\n\n";

    std::cout
    << classification.label
    << "\n";

    std::cout
    << "Confidence: "
    << classification.confidence
    << "%\n";

    for (const auto& e : classification.evidence)
    {
        std::cout
        << "- "
        << e
        << "\n";
    }
}

void ConsoleReporter::print_optimizer(
    const KernelOptimization& opt
)
{
    std::cout
    << "\n========================================\n"
    << "OPTIMIZATION\n"
    << "========================================\n\n";

    for (const auto& r : opt.recommendations)
    {
        std::cout
        << "- "
        << r
        << "\n";
    }
}

void ConsoleReporter::print_ir_summary(
    const KernelIR& ir
)
{
    std::cout
    << "\n========================================\n"
    << "KERNEL IR\n"
    << "========================================\n\n";

    std::cout
    << "Kernel: "
    << ir.kernel
    << "\n";

    std::cout
    << "Classification: "
    << ir.classification.label
    << "\n";

    std::cout
    << "Utilization: "
    << ir.state.utilization_score
    << "%\n";
}

void ConsoleReporter::print_workflow(
    const KernelWorkflow& workflow
)
{
    std::cout
    << "\n========================================\n"
    << "WORKFLOW\n"
    << "========================================\n\n";

    std::cout
    << "Type: "
    << workflow.type
    << "\n";

    std::cout
    << "Summary: "
    << workflow.summary
    << "\n";
}
