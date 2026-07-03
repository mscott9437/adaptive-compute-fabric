#include <iostream>
#include <vector>
#include <filesystem>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <cuda_runtime.h>

#include "../include/benchmark_runner.hpp"

#include "../include/kernel_report.hpp"

#include "../include/json_exporter.hpp"
#include "../include/csv_exporter.hpp"

#include "../include/console_reporter.hpp"

#include "../include/history_loader.hpp"
#include "../include/benchmark_comparator.hpp"
#include "../include/benchmark_history.hpp"

#include "../include/nsight_capture.hpp"
#include "../device/nsight_profiler.hpp"

void create_results_directories()
{
    std::filesystem::create_directories(
        "results"
    );

    std::filesystem::create_directories(
        "results/vector_add"
    );

    std::filesystem::create_directories(
        "results/reduction"
    );
}

void print_device_info()
{
    cudaDeviceProp prop;

    cudaGetDeviceProperties(
        &prop,
        0
    );

    std::cout
        << "GPU: "
        << prop.name
        << "\n";

    std::cout
        << "Compute Capability: "
        << prop.major
        << "."
        << prop.minor
        << "\n";

    std::cout
        << "Global Memory: "
        << prop.totalGlobalMem
            / (1024 * 1024)
        << " MB\n\n";
}

void print_final_summary(
    const KernelReport& vector_add,
    const KernelReport& reduction
)
{
    std::cout
        << "\n========================================\n";

    std::cout
        << "FINAL SUMMARY\n";

    std::cout
        << "========================================\n\n";

    std::cout
        << "Best Vector Add Block Size: "
        << vector_add.tested_block_size
        << "\n";

    std::cout
        << "Vector Add Runtime: "
        << vector_add.runtime_ms
        << " ms\n";

    std::cout
        << "Vector Add Bandwidth: "
        << vector_add.bandwidth_gb_s
        << " GB/s\n\n";

    std::cout
        << "Best Reduction Block Size: "
        << reduction.tested_block_size
        << "\n";

    std::cout
        << "Reduction Runtime: "
        << reduction.runtime_ms
        << " ms\n";

    std::cout
        << "Reduction Occupancy: "
        << reduction.occupancy
        << "\n";
}

static void print_nsight_comparison(
    const std::string& kernel,
    const KernelReport& winner,
    const KernelReport& runner_up
)
{
    std::cout
        << "\n========================================\n";

    std::cout
        << "NSIGHT COMPARISON\n";

    std::cout
        << "========================================\n\n";

    std::cout
        << "Kernel: "
        << kernel
        << "\n\n";

    std::cout
        << "Winner Block Size: "
        << winner.tested_block_size
        << "\n";

    std::cout
        << "Runner-Up Block Size: "
        << runner_up.tested_block_size
        << "\n\n";

    std::cout
        << "Runtime Delta: "
        << (
            runner_up.runtime_ms
            - winner.runtime_ms
        )
        << " ms\n\n";

    std::cout
        << "Winner SM Throughput: "
        << winner.sm_throughput_pct
        << "%\n";

    std::cout
        << "Runner-Up SM Throughput: "
        << runner_up.sm_throughput_pct
        << "%\n\n";

    std::cout
        << "Winner Warps Eligible: "
        << winner.warps_eligible_pct
        << "%\n";

    std::cout
        << "Runner-Up Warps Eligible: "
        << runner_up.warps_eligible_pct
        << "%\n\n";

    std::cout
        << "Winner Long Scoreboard Stall: "
        << winner.stall_long_scoreboard_pct
        << "%\n";

    std::cout
        << "Runner-Up Long Scoreboard Stall: "
        << runner_up.stall_long_scoreboard_pct
        << "%\n";
}

static std::string generate_session_id()
{
    auto now =
        std::chrono::system_clock::now();

    std::time_t t =
        std::chrono::system_clock::to_time_t(now);

    std::tm tm =
        *std::localtime(&t);

    std::ostringstream oss;

    oss
        << std::put_time(
               &tm,
               "%Y%m%d_%H%M%S"
           );

    return oss.str();
}

int main(
    int argc,
    char** argv
)
{
	if (
    	argc >= 3 &&
    	std::string(argv[1]) ==
    	"--profile-vector-add"
	)
	{
    	run_profile_vector_add(
        	std::stoi(argv[2])
    	);
	
    	return 0;
	}
	
	if (
    	argc >= 3 &&
    	std::string(argv[1]) ==
    	"--profile-reduction"
	)
	{
    	run_profile_reduction(
        	std::stoi(argv[2])
    	);
	
    	return 0;
	}

	if (
    	argc > 1 &&
    	std::string(argv[1]) == "--test-nsight"
	)
	{
    	NsightProfiler profiler;
	
    	auto metrics =
        	profiler.profile_vector_add(
            	512
        	);
	
    	std::cout
        	<< "\n=== NSIGHT TEST ===\n";
	
    	std::cout
        	<< "Valid: "
        	<< metrics.valid
        	<< "\n";
	
    	std::cout
        	<< "DRAM Throughput: "
        	<< metrics.dram_throughput_pct
        	<< "\n";
	
    	std::cout
        	<< "Compute Memory Throughput: "
        	<< metrics.compute_memory_throughput_pct
        	<< "\n";
	
    	std::cout
        	<< "GPU Time (ns): "
        	<< metrics.gpu_time_ns
        	<< "\n";
	
    	std::cout
        	<< "Max Warps %: "
        	<< metrics.max_warps_pct
        	<< "\n";
	
    	return 0;
	}

	const std::string session_id =
    	generate_session_id();
    
    create_results_directories();

    print_device_info();

    BenchmarkRunner runner;

    constexpr int elements =
        1'000'000;

    std::vector<int> launch_configs =
    {
        64,
        128,
        256,
        512,
        768,
        1024
    };

	std::vector<int> reduction_configs =
	{
    	64,
    	128,
    	256,
    	512,
    	1024
	};
	
	auto previous_vector_add =
    	HistoryLoader::load_best(
        	"results/vector_add/history.csv"
    	);

	auto best_vector_add =
    	runner.sweep_vector_add(
        	elements,
        	launch_configs,
        	session_id
    	);
	
	auto runner_up_vector_add =
    	runner.vector_add_runner_up();

	std::cout
    	<< "\nDEBUG VECTOR\n"
    	<< "Winner: "
    	<< best_vector_add.tested_block_size
    	<< "\nRunner-Up: "
    	<< runner_up_vector_add.tested_block_size
    	<< "\n";
    
	if (previous_vector_add)
	{
    	BenchmarkComparator::compare(
        	*previous_vector_add,
        	best_vector_add
    	);
	}

	auto profiled_vector_add =
    	runner.profile_vector_add_winner(
        	best_vector_add
    	);
	
	auto profiled_runner_up_vector_add =
    	runner.profile_vector_add_runner_up(
        	runner_up_vector_add
    	);
	
	ConsoleReporter::print_nsight_summary(
    	profiled_vector_add
	);
	
	ConsoleReporter::print_nsight_summary(
    	profiled_runner_up_vector_add
	);
	
	print_nsight_comparison(
    	"vector_add",
    	profiled_vector_add,
    	profiled_runner_up_vector_add
	);

	ConsoleReporter::print_winner_comparison(
    		profiled_vector_add,
    		profiled_runner_up_vector_add
		);
	
	export_csv_row(
    	best_vector_add,
    	"results/vector_add/history.csv"
	);

	auto previous_reduction =
    	HistoryLoader::load_best(
        	"results/reduction/history.csv"
    	);

	auto best_reduction =
    	runner.sweep_reduction(
        	elements,
        	reduction_configs,
        	session_id
    	);
	
	auto runner_up_reduction =
    	runner.reduction_runner_up();
	
	std::cout
    	<< "\nDEBUG REDUCTION\n"
    	<< "Winner: "
    	<< best_reduction.tested_block_size
    	<< "\nRunner-Up: "
    	<< runner_up_reduction.tested_block_size
    	<< "\n";
    
	if (previous_reduction)
	{
    	BenchmarkComparator::compare(
        	*previous_reduction,
        	best_reduction
    	);
	}

	auto profiled_reduction =
    	runner.profile_reduction_winner(
        	best_reduction
    	);
	
	auto profiled_runner_up_reduction =
    	runner.profile_reduction_runner_up(
        	runner_up_reduction
    	);
	
	ConsoleReporter::print_nsight_summary(
    	profiled_reduction
	);
	
	ConsoleReporter::print_nsight_summary(
    	profiled_runner_up_reduction
	);
	
	print_nsight_comparison(
    	"reduction",
    	profiled_reduction,
    	profiled_runner_up_reduction
	);

	ConsoleReporter::print_winner_comparison(
    	profiled_reduction,
    	profiled_runner_up_reduction
	);

	export_csv_row(
    	best_reduction,
    	"results/reduction/history.csv"
	);

    print_final_summary(
        best_vector_add,
        best_reduction
    );
    
    auto full_history =
        BenchmarkHistory::load_csv(
            "results/vector_add/full_history.csv"
        );

    auto winner_history =
        BenchmarkHistory::load_csv(
            "results/vector_add/history.csv"
        );

	auto tested_frequency =
    	BenchmarkHistory::
    	most_frequently_tested_block_size(
        	full_history
    	);
	
	if (tested_frequency.tie_detected)
	{
    	std::cout
        	<< "Most Common Tested Block Size: none "
        	<< "(all tested equally)\n";
	}
	else
	{
    	std::cout
        	<< "Most Common Tested Block Size: "
        	<< tested_frequency.block_size
        	<< "\n";
	}

	std::cout
    	<< "Most Common Winning Block Size: "
    	<< BenchmarkHistory::most_common_winning_block_size(
           	winner_history
       	)
    	<< "\n";
    
    auto gpus =
        BenchmarkHistory::unique_gpus(
            full_history
        );

    for (
        const auto& gpu
        : gpus
    )
    {
        auto reports =
            BenchmarkHistory::filter_by_gpu(
                full_history,
                gpu
            );

        auto best_runtime =
            BenchmarkHistory::best_runtime(
                reports
            );

        auto best_efficiency =
            BenchmarkHistory::best_efficiency(
                reports
            );

        std::cout
            << "\nGPU: "
            << gpu
            << "\n";

        std::cout
            << "Best Runtime: "
            << best_runtime.runtime_ms
            << " ms\n";

        std::cout
            << "Best Efficiency: "
            << best_efficiency.efficiency_score
            << "\n";

        std::cout
            << "Best Occupancy: "
            << (
                   BenchmarkHistory::best_occupancy(
                       reports
                   )
                   * 100.0f
               )
            << "%\n";
    }

    std::cout
        << "\nFull History Entries: "
        << full_history.size()
        << "\n";

    auto best =
        BenchmarkHistory::best_runtime(
            full_history
        );

    auto most_efficient =
        BenchmarkHistory::best_efficiency(
            full_history
        );

    auto best_occupancy_report =
        BenchmarkHistory::best_occupancy_report(
            full_history
        );

    std::cout
        << "Best Efficiency Score: "
        << most_efficient.efficiency_score
        << "\n";

    std::cout
        << "Efficiency Block Size: "
        << most_efficient.tested_block_size
        << "\n";

    std::cout
        << "Occupancy Block Size: "
        << best_occupancy_report.tested_block_size
        << "\n";

    std::cout
        << "Average Efficiency: "
        << BenchmarkHistory::average_efficiency(
               full_history
           )
        << "\n";

    std::cout
        << "Best Occupancy: "
        << (
               BenchmarkHistory::best_occupancy(
                   full_history
               )
               * 100.0f
           )
        << "%\n";

    std::cout
        << "Average Occupancy: "
        << (
               BenchmarkHistory::average_occupancy(
                   full_history
               )
               * 100.0f
           )
        << "%\n";

    std::cout
        << "Historical Best Runtime: "
        << best.runtime_ms
        << " ms\n";

    std::cout
        << "Average Runtime: "
        << BenchmarkHistory::average_runtime(
               full_history
           )
        << " ms\n";
    
    return 0;
}
