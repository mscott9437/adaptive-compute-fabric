#include "../include/csv_exporter.hpp"

#include <fstream>
#include <filesystem>

void export_csv_row(
    const KernelReport& report,
    const std::string& path
)
{
    bool write_header =
        !std::filesystem::exists(path);

    std::ofstream out(
        path,
        std::ios::app
    );

    if (write_header)
    {
		out
		<< "schema_version,"
		<< "timestamp,"
		<< "session_id,"
		<< "gpu_name,"
		<< "gpu_uuid,"
		<< "compute_capability,"
		<< "kernel,"
		<< "block_size,"
		<< "grid_size,"
		<< "runtime_ms,"
		<< "bandwidth_gb_s,"
		<< "occupancy,"
		<< "registers_per_thread,"
		<< "shared_memory_bytes,"
		<< "active_blocks_per_sm,"
		<< "efficiency_score,"
		<< "dram_throughput_pct,"
		<< "compute_memory_throughput_pct,"
		<< "nsight_kernel_time_ms,"
		<< "max_warps_active_pct,"
		
		<< "sm_throughput_pct,"
		<< "warps_active_pct,"
		<< "warps_eligible_pct,"
		
		<< "stall_long_scoreboard_pct,"
		<< "stall_short_scoreboard_pct,"
		
		<< "stall_not_selected_pct,"
		<< "stall_barrier_pct,"
		
		<< "kernel_classification,"
		<< "classification_confidence\n";
    }

		out
		<< report.schema_version << ","
		<< report.timestamp << ","
		<< report.session_id << ","
		<< report.gpu_name << ","
		<< report.gpu_uuid << ","
		<< report.compute_capability << ","
		<< report.kernel << ","
		<< report.tested_block_size << ","
		<< report.tested_grid_size << ","
		<< report.runtime_ms << ","
		<< report.bandwidth_gb_s << ","
		<< report.occupancy << ","
		<< report.registers_per_thread << ","
		<< report.shared_memory_bytes << ","
		<< report.active_blocks_per_sm << ","
		<< report.efficiency_score << ","
		<< report.dram_throughput_pct << ","
		<< report.compute_memory_throughput_pct << ","
		<< report.nsight_kernel_time_ms << ","
		<< report.max_warps_active_pct << ","
		
		<< report.sm_throughput_pct << ","
		<< report.warps_active_pct << ","
		<< report.warps_eligible_pct << ","
		
		<< report.stall_long_scoreboard_pct << ","
		<< report.stall_short_scoreboard_pct << ","
		
		<< report.stall_not_selected_pct << ","
		<< report.stall_barrier_pct << ","
		
		<< report.kernel_classification << ","
		<< report.classification_confidence
		
		<< "\n";
}
