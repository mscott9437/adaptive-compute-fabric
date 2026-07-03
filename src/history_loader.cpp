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
