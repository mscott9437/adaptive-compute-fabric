#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct KernelReport
{
	std::string backend;
	std::string kernel;

	bool passed = false;
	
	size_t elements = 0;
	
	double runtime_ms = 0.0;
	double bandwidth_gb_s = 0.0;
	double throughput_gops = 0.0;
	
	double bytes_processed = 0.0;
	
	float occupancy = 0.0f;
	
	int active_blocks_per_sm = 0;
	
	int recommended_block_size = 0;
	int recommended_grid_size = 0;
	
	int registers_per_thread = 0;
	size_t shared_memory_bytes = 0;
	
	std::string gpu_name;
	std::string compute_capability;
	
	int global_memory_mb = 0;

	int tested_block_size = 0;
	
	int tested_grid_size = 0;
	
	double efficiency_score = 0.0;

	int schema_version = 2;

	std::string timestamp;
	std::string session_id;
	
	std::string gpu_uuid;

	//
	// Nsight Compute Metrics
	//
	
	double dram_throughput_pct = 0.0;
	
	double compute_memory_throughput_pct = 0.0;
	
	double nsight_kernel_time_ms = 0.0;
	
	double max_warps_active_pct = 0.0;
	
	//
	// Scheduler metrics
	//
	
	double sm_throughput_pct = 0.0;
	
	double warps_active_pct = 0.0;
		
	double warps_eligible_pct = 0.0;
	
	double stall_long_scoreboard_pct = 0.0;
	
	double stall_short_scoreboard_pct = 0.0;
	
	double stall_not_selected_pct = 0.0;
	
	double stall_barrier_pct = 0.0;
	
	bool has_nsight_metrics = false;
	
	std::string kernel_classification;
	
	double classification_confidence = 0.0;
	
	std::vector<std::string>
    	classification_evidence;
};
