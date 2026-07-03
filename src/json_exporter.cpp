#include "../include/json_exporter.hpp"

#include <fstream>

void export_json(
    const KernelReport& report,
    const std::string& path
)
{
    std::ofstream out(path);

    out
        << "{\n"

        << "  \"backend\": \""
        << report.backend
        << "\",\n"

        << "  \"kernel\": \""
        << report.kernel
        << "\",\n"

        << "  \"passed\": "
        << (report.passed ? "true" : "false")
        << ",\n"

        << "  \"elements\": "
        << report.elements
        << ",\n"

        << "  \"runtime_ms\": "
        << report.runtime_ms
        << ",\n"

        << "  \"bandwidth_gb_s\": "
        << report.bandwidth_gb_s
        << ",\n"

        << "  \"throughput_gops\": "
        << report.throughput_gops
        << ",\n"

        << "  \"bytes_processed\": "
        << report.bytes_processed
        << ",\n"

        << "  \"occupancy\": "
        << report.occupancy
        << ",\n"

        << "  \"active_blocks_per_sm\": "
        << report.active_blocks_per_sm
        << ",\n"

		<< "  \"tested_block_size\": "
		<< report.tested_block_size
		<< ",\n"
		
		<< "  \"tested_grid_size\": "
		<< report.tested_grid_size
		<< ",\n"

        << "  \"recommended_block_size\": "
        << report.recommended_block_size
        << ",\n"

        << "  \"recommended_grid_size\": "
        << report.recommended_grid_size
        << ",\n"

        << "  \"registers_per_thread\": "
        << report.registers_per_thread
        << ",\n"

        << "  \"shared_memory_bytes\": "
        << report.shared_memory_bytes
        << ",\n"

        << "  \"gpu_name\": \""
        << report.gpu_name
        << "\",\n"

        << "  \"compute_capability\": \""
        << report.compute_capability
        << "\",\n"

		<< "  \"global_memory_mb\": "
		<< report.global_memory_mb
		<< ",\n"

		<< "  \"schema_version\": "
		<< report.schema_version
		<< ",\n"
		
		<< "  \"timestamp\": \""
		<< report.timestamp
		<< "\",\n"
		
		<< "  \"gpu_uuid\": \""
		<< report.gpu_uuid
		<< "\",\n"

		<< "  \"efficiency_score\": "
		<< report.efficiency_score
		<< ",\n"
		
		<< "  \"dram_throughput_pct\": "
		<< report.dram_throughput_pct
		<< ",\n"
		
		<< "  \"compute_memory_throughput_pct\": "
		<< report.compute_memory_throughput_pct
		<< ",\n"
		
		<< "  \"nsight_kernel_time_ms\": "
		<< report.nsight_kernel_time_ms
		<< ",\n"
		
		<< "  \"max_warps_active_pct\": "
		<< report.max_warps_active_pct
		<< ",\n"
		
		<< "  \"warps_active_pct\": "
		<< report.warps_active_pct
		<< ",\n"
		
		<< "  \"sm_throughput_pct\": "
		<< report.sm_throughput_pct
		<< ",\n"
		
		<< "  \"warps_eligible_pct\": "
		<< report.warps_eligible_pct
		<< ",\n"
		
		<< "  \"stall_long_scoreboard_pct\": "
		<< report.stall_long_scoreboard_pct
		<< ",\n"
		
		<< "  \"stall_short_scoreboard_pct\": "
		<< report.stall_short_scoreboard_pct
		<< ",\n"
		
		<< "  \"stall_not_selected_pct\": "
		<< report.stall_not_selected_pct
		<< ",\n"
		
		<< "  \"stall_barrier_pct\": "
		<< report.stall_barrier_pct
		<< ",\n"
		
		<< "  \"kernel_classification\": \""
		<< report.kernel_classification
		<< "\",\n"
		
		<< "  \"classification_confidence\": "
		<< report.classification_confidence
		<< "\n"
		
		<< "}\n";
}
