#include "../include/history_loader.hpp"

#include <fstream>
#include <sstream>

std::optional<KernelReport>
HistoryLoader::load_best(
    const std::string& path
)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        return std::nullopt;
    }

    std::string line;

    std::getline(
        in,
        line
    );

    KernelReport best;

    bool found = false;

    while (
        std::getline(
            in,
            line
        )
    )
    {
        std::stringstream ss(line);

        std::string token;

        KernelReport current;

        std::getline(
            ss,
            token,
            ','
        );

        current.schema_version =
            std::stoi(token);

		std::getline(
    		ss,
    		current.timestamp,
    		','
		);

		std::getline(
    		ss,
    		current.session_id,
    		','
		);

		std::getline(
    		ss,
    		current.gpu_name,
    		','
		);

		std::getline(
    		ss,
    		current.gpu_uuid,
    		','
		);

		std::getline(
    		ss,
    		current.compute_capability,
    		','
		);
		
		std::getline(
    		ss,
    		current.kernel,
    		','
		);

        std::getline(
            ss,
            token,
            ','
        );

        current.tested_block_size =
            std::stoi(token);

        std::getline(
            ss,
            token,
            ','
        );

        current.tested_grid_size =
            std::stoi(token);

        std::getline(
            ss,
            token,
            ','
        );

        current.runtime_ms =
            std::stof(token);

		std::getline(ss, token, ',');
		current.bandwidth_gb_s = std::stod(token);
		
		std::getline(ss, token, ',');
		current.occupancy = std::stof(token);
		
		std::getline(ss, token, ',');
		current.registers_per_thread = std::stoi(token);
		
		std::getline(ss, token, ',');
		current.shared_memory_bytes = std::stoull(token);
		
		std::getline(ss, token, ',');
		current.active_blocks_per_sm = std::stoi(token);
		
		std::getline(ss, token, ',');
		current.efficiency_score = std::stod(token);

        std::getline(ss, token, ',');
        current.dram_throughput_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.compute_memory_throughput_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.nsight_kernel_time_ms =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.max_warps_active_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.sm_throughput_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.warps_active_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.warps_eligible_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.stall_long_scoreboard_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.stall_short_scoreboard_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.stall_not_selected_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, token, ',');
        current.stall_barrier_pct =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, current.kernel_classification, ',');

        std::getline(ss, token, ',');
        current.classification_confidence =
        token.empty() ? 0.0 : std::stod(token);

        std::getline(ss, current.workflow, ',');

        std::getline(ss, token, ',');
        current.memory_pressure =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.execution_pressure =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.scheduler_pressure =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.cache_pressure =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.utilization_score =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.cache_hit_ratio =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.fp32_ratio =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.integer_ratio =
        token.empty() ? 0.0f : std::stof(token);

        std::getline(ss, token, ',');
        current.memory_ratio =
        token.empty() ? 0.0f : std::stof(token);

        if (
            !found
            ||
            current.runtime_ms
            <
            best.runtime_ms
        )
        {
            best =
                current;

            found =
                true;
        }
    }

    if (!found)
    {
        return std::nullopt;
    }

    return best;
}
