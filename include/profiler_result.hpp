#pragma once

#include <string>

struct ProfilerResult
{
    std::string backend;
    std::string kernel;

    float occupancy = 0.0f;

    int recommended_block_size = 0;
    int recommended_grid_size = 0;

    int registers_per_thread = 0;

    size_t shared_memory_bytes = 0;

    int active_blocks_per_sm = 0;

    float runtime_ms = 0.0f;

    float warp_efficiency = 0.0f;

    float l2_hit_rate = 0.0f;

    float l2_throughput_gb_s = 0.0f;

    size_t global_memory_bytes = 0;
    
    double dram_throughput_pct = 0.0;
	double compute_memory_throughput_pct = 0.0;
	double nsight_kernel_time_ms = 0.0;
	double max_warps_pct = 0.0;
};
