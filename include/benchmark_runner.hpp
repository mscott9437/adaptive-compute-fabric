#pragma once

#include <vector>

#include "kernel_report.hpp"
#include "kernel_analysis.hpp"

class BenchmarkRunner
{
public:

	KernelReport run_vector_add(
    	int elements,
    	int block_size,
    	const std::string& session_id
	);
	
	KernelReport run_reduction(
    	int elements,
    	int block_size,
    	const std::string& session_id
	);
	
	KernelReport sweep_vector_add(
    	int elements,
    	const std::vector<int>& launch_configs,
    	const std::string& session_id
	);
	
	KernelReport sweep_reduction(
    	int elements,
    	const std::vector<int>& launch_configs,
    	const std::string& session_id
	);

	KernelAnalysis
	profile_vector_add_winner(
		KernelReport&
	);

	KernelAnalysis
	profile_vector_add_runner_up(
		KernelReport&
	);

	KernelAnalysis
	profile_reduction_winner(
		KernelReport&
	);

	KernelAnalysis
	profile_reduction_runner_up(
		KernelReport&
	);

	KernelReport
	vector_add_runner_up() const;

	KernelReport
	reduction_runner_up() const;
	
private:

    std::vector<KernelReport>
        last_vector_add_results;

    std::vector<KernelReport>
        last_reduction_results;

	KernelAnalysis analyze_kernel(
		KernelReport report
	);
};
