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
	
	float inst_executed_per_cycle_active = 0.0f;

	float stall_math_pipe_pct = 0.0f;
	
	float stall_mio_pct = 0.0f;
	
	float stall_tex_pct = 0.0f;
    //
    // Execution
    //

    double inst_executed_per_cycle_active_pct = 0.0;

    double stall_tex_pipe_pct = 0.0;

    //
    // Instruction mix
    //

    double inst_fp32 = 0.0;

    double inst_fp64 = 0.0;

    double inst_integer = 0.0;

    double inst_memory = 0.0;

    //
    // Cache
    //

    double l1_hit_rate_pct = 0.0;

    double l2_hit_rate_pct = 0.0;

    double l1_throughput_pct = 0.0;

    double l2_throughput_pct = 0.0;

    float l1_global_load_requests = 0.0f;

    float l1_global_store_requests = 0.0f;

    float l2_sector_average = 0.0f;

    float l2_requests = 0.0f;

    float l2_hits = 0.0f;

    float l2_misses = 0.0f;

};
