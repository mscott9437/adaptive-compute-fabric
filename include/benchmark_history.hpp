#pragma once

#include "kernel_report.hpp"

#include <vector>
#include <string>

struct BlockSizeFrequencyResult
{
    int block_size = 0;
    int count = 0;
    bool tie_detected = false;
};

class BenchmarkHistory
{
public:

    static std::vector<KernelReport>
    load_csv(
        const std::string& path
    );

    static KernelReport
    best_runtime(
        const std::vector<KernelReport>& reports
    );

    static double
    average_runtime(
        const std::vector<KernelReport>& reports
    );

    static double
    average_bandwidth(
        const std::vector<KernelReport>& reports
    );
    
    static KernelReport
	best_efficiency(
    	const std::vector<KernelReport>& reports
	);

	static double
	average_efficiency(
    	const std::vector<KernelReport>& reports
	);

	static float
	best_occupancy(
    	const std::vector<KernelReport>& reports
	);
	
	static float
	average_occupancy(
    	const std::vector<KernelReport>& reports
	);
	
	static BlockSizeFrequencyResult
	most_frequently_tested_block_size(
    	const std::vector<KernelReport>& reports
	);

	static std::vector<KernelReport>
	filter_by_gpu(
    	const std::vector<KernelReport>& reports,
    	const std::string& gpu_name
	);

	static std::vector<std::string>
	unique_gpus(
    	const std::vector<KernelReport>& reports
	);
	
	static int
	best_efficiency_block_size(
    	const std::vector<KernelReport>& reports
	);
	
	static int
	best_occupancy_block_size(
    	const std::vector<KernelReport>& reports
	);

	static KernelReport
	best_occupancy_report(
    	const std::vector<KernelReport>& reports
	);

	static int
	most_common_winning_block_size(
    	const std::vector<KernelReport>& reports
	);

	static double average_memory_pressure(
		const std::vector<KernelReport>& reports
	);

	static double average_execution_pressure(
		const std::vector<KernelReport>& reports
	);

	static double average_scheduler_pressure(
		const std::vector<KernelReport>& reports
	);

	static double average_cache_hit_ratio(
		const std::vector<KernelReport>& reports
	);

	static std::string most_common_workflow(
		const std::vector<KernelReport>& reports
	);

	static std::string most_common_classification(
		const std::vector<KernelReport>& reports
	);
};
