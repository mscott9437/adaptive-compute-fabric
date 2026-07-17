#pragma once

#include <string>

struct NsightMetrics
{
    bool valid = false;

    //
    // Existing
    //

    double dram_throughput_pct = 0.0;
    double compute_memory_throughput_pct = 0.0;

    double gpu_time_ns = 0.0;

    double max_warps_pct = 0.0;

    double sm_throughput_pct = 0.0;

    double warps_active_pct = 0.0;
    double warps_eligible_pct = 0.0;

    double stall_long_scoreboard_pct = 0.0;
    double stall_short_scoreboard_pct = 0.0;
    double stall_not_selected_pct = 0.0;
    double stall_barrier_pct = 0.0;

    //
    // NEW execution metrics
    //

    double inst_executed_per_cycle_active_pct = 0.0;

    double stall_math_pipe_pct = 0.0;

    double stall_mio_pct = 0.0;

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

    // L1 cache

    float l1_global_load_requests = 0.0f;

    float l1_global_store_requests = 0.0f;


    // L2 cache

    float l2_sector_average = 0.0f;

    float l2_requests = 0.0f;

    float l2_hits = 0.0f;

    float l2_misses = 0.0f;
};

class NsightProfiler
{
public:

	NsightMetrics profile_vector_add(
    	int block_size
	);

	NsightMetrics profile_reduction(
    	int block_size
	);

private:

    std::string run_command(
        const std::string& command
    );

    NsightMetrics parse_csv(
        const std::string& csv_output
    );

    NsightMetrics run_profile_command(
        const std::string& target
    );

};
