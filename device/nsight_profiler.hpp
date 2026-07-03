#pragma once

#include <string>

struct NsightMetrics
{
    double dram_throughput_pct = 0.0;

    double compute_memory_throughput_pct = 0.0;

    double gpu_time_ns = 0.0;

    double max_warps_pct = 0.0;

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

    bool valid = false;
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
