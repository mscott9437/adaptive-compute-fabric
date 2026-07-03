#pragma once

#include <vector>

#include "kernel_report.hpp"

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

	KernelReport profile_vector_add_winner(
    	KernelReport& report
	);
	
	KernelReport profile_reduction_winner(
    	KernelReport& report
	);

	KernelReport
	vector_add_runner_up() const;
	
	KernelReport
	reduction_runner_up() const;
	
	KernelReport profile_vector_add_runner_up(
    	KernelReport& report
	);
	
	KernelReport profile_reduction_runner_up(
    	KernelReport& report
	);
	
private:

    std::vector<KernelReport>
        last_vector_add_results;

    std::vector<KernelReport>
        last_reduction_results;
};
