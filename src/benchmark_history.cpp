#include "../include/benchmark_history.hpp"

#include <fstream>
#include <sstream>
#include <limits>
#include <unordered_map>
#include <set>
#include <iostream>

std::vector<KernelReport>
BenchmarkHistory::load_csv(
    const std::string& path
)
{
    std::vector<KernelReport> reports;

    std::ifstream in(path);

    if (!in.is_open())
    {
        return reports;
    }

    std::string line;

    std::getline(
        in,
        line
    );

	while (
    	std::getline(
        	in,
        	line
    	)
	)
	{
    	try
    	{
        	std::stringstream ss(line);
	
        	KernelReport report;
	
        	std::string token;
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	if (token.empty())
        	{
            	continue;
        	}
	
        	report.schema_version =
            	std::stoi(token);
    	
			std::getline(
    			ss,
    			report.timestamp,
    			','
			);
	
			std::getline(
    			ss,
    			report.session_id,
    			','
			);
	
			std::getline(
    			ss,
    			report.gpu_name,
    			','
			);
			
			std::getline(
    			ss,
    			report.gpu_uuid,
    			','
			);
			
			std::getline(
    			ss,
    			report.compute_capability,
    			','
			);
			
			std::getline(
    			ss,
    			report.kernel,
    			','
			);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.tested_block_size =
            	std::stoi(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.tested_grid_size =
            	std::stoi(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.runtime_ms =
            	std::stod(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.bandwidth_gb_s =
            	std::stod(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.occupancy =
            	std::stof(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
        	report.registers_per_thread =
            	std::stoi(token);
	
        	std::getline(
            	ss,
            	token,
            	','
        	);
	
			report.shared_memory_bytes =
    			std::stoull(token);
	
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.active_blocks_per_sm =
    			std::stoi(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
	
			report.efficiency_score =
    			std::stod(token);
	
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.dram_throughput_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.compute_memory_throughput_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.nsight_kernel_time_ms =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.max_warps_active_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.sm_throughput_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.warps_active_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.warps_eligible_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.stall_long_scoreboard_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.stall_short_scoreboard_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.stall_not_selected_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
			
			std::getline(
    			ss,
    			token,
    			','
			);
			
			report.stall_barrier_pct =
    			token.empty()
    			? 0.0
    			: std::stod(token);
    	
		reports.push_back(
    		report
		);
    		}
    		catch (
        		const std::exception& e
    		)
    		{
        		std::cerr
            		<< "Failed to parse history row: "
            		<< e.what()
            		<< "\n";
		
        		continue;
    		}
		}

    return reports;
}

KernelReport
BenchmarkHistory::best_runtime(
    const std::vector<KernelReport>& reports
)
{
    KernelReport best;

    bool first = true;

    for (
        const auto& report
        : reports
    )
    {
        if (
            first
            ||
            report.runtime_ms
            <
            best.runtime_ms
        )
        {
            best =
                report;

            first =
                false;
        }
    }

    return best;
}

double
BenchmarkHistory::average_runtime(
    const std::vector<KernelReport>& reports
)
{
    if (
        reports.empty()
    )
    {
        return 0.0;
    }

    double total = 0.0;

    for (
        const auto& report
        : reports
    )
    {
        total +=
            report.runtime_ms;
    }

    return
        total
        /
        reports.size();
}

double
BenchmarkHistory::average_bandwidth(
    const std::vector<KernelReport>& reports
)
{
    if (
        reports.empty()
    )
    {
        return 0.0;
    }

    double total = 0.0;

    for (
        const auto& report
        : reports
    )
    {
        total +=
            report.bandwidth_gb_s;
    }

    return
        total
        /
        reports.size();
}

float
BenchmarkHistory::best_occupancy(
    const std::vector<KernelReport>& reports
)
{
    float best = 0.0f;

    for (
        const auto& report
        : reports
    )
    {
        if (
            report.occupancy
            > best
        )
        {
            best =
                report.occupancy;
        }
    }

    return best;
}

float
BenchmarkHistory::average_occupancy(
    const std::vector<KernelReport>& reports
)
{
    if (reports.empty())
    {
        return 0.0f;
    }

    float total = 0.0f;

    for (
        const auto& report
        : reports
    )
    {
        total +=
            report.occupancy;
    }

    return
        total
        /
        reports.size();
}

BlockSizeFrequencyResult
BenchmarkHistory::most_frequently_tested_block_size(
    const std::vector<KernelReport>& reports
)
{
    BlockSizeFrequencyResult result;

    if (reports.empty())
    {
        return result;
    }

    std::unordered_map<int,int> counts;

    for (const auto& report : reports)
    {
        counts[
            report.tested_block_size
        ]++;
    }

    int best_count = 0;

    for (const auto& pair : counts)
    {
        if (pair.second > best_count)
        {
            best_count =
                pair.second;

            result.block_size =
                pair.first;

            result.count =
                pair.second;

            result.tie_detected =
                false;
        }
        else if (
            pair.second ==
            best_count
        )
        {
            result.tie_detected =
                true;
        }
    }

    return result;
}

KernelReport
BenchmarkHistory::best_efficiency(
    const std::vector<KernelReport>& reports
)
{
    KernelReport best;

    bool first = true;

    for (
        const auto& report
        : reports
    )
    {
        if (
            first
            ||
            report.efficiency_score >
            best.efficiency_score
        )
        {
            best =
                report;

            first = false;
        }
    }

    return best;
}

int
BenchmarkHistory::best_efficiency_block_size(
    const std::vector<KernelReport>& reports
)
{
    if (reports.empty())
    {
        return 0;
    }

    const KernelReport* best =
        &reports.front();

    for (
        const auto& report
        : reports
    )
    {
        if (
            report.efficiency_score >
            best->efficiency_score
        )
        {
            best =
                &report;
        }
    }

    return
        best->tested_block_size;
}

int
BenchmarkHistory::best_occupancy_block_size(
    const std::vector<KernelReport>& reports
)
{
    if (reports.empty())
    {
        return 0;
    }

    const KernelReport* best =
        &reports.front();

    for (
        const auto& report
        : reports
    )
    {
        if (
            report.occupancy >
            best->occupancy
        )
        {
            best =
                &report;
        }
    }

    return
        best->tested_block_size;
}

double
BenchmarkHistory::average_efficiency(
    const std::vector<KernelReport>& reports
)
{
    if (reports.empty())
    {
        return 0.0;
    }

    double total = 0.0;

    for (
        const auto& report
        : reports
    )
    {
        total +=
            report.efficiency_score;
    }

    return
        total
        /
        reports.size();
}

std::vector<KernelReport>
BenchmarkHistory::filter_by_gpu(
    const std::vector<KernelReport>& reports,
    const std::string& gpu_name
)
{
    std::vector<KernelReport> filtered;

    for (
        const auto& report
        : reports
    )
    {
        if (
            report.gpu_name
            == gpu_name
        )
        {
            filtered.push_back(
                report
            );
        }
    }

    return filtered;
}

std::vector<std::string>
BenchmarkHistory::unique_gpus(
    const std::vector<KernelReport>& reports
)
{
    std::set<std::string> names;

    for (
        const auto& report
        : reports
    )
    {
        names.insert(
            report.gpu_name
        );
    }

	return {
    	names.begin(),
    	names.end()
	};
}

KernelReport
BenchmarkHistory::best_occupancy_report(
    const std::vector<KernelReport>& reports
)
{
    KernelReport best;

    bool first = true;

    for (
        const auto& report
        : reports
    )
    {
        if (
            first
            ||
            report.occupancy >
            best.occupancy
        )
        {
            best = report;
            first = false;
        }
        else if (
            report.occupancy ==
            best.occupancy
            &&
            report.runtime_ms <
            best.runtime_ms
        )
        {
            best = report;
        }
    }

    return best;
}

int
BenchmarkHistory::most_common_winning_block_size(
    const std::vector<KernelReport>& reports
)
{
    std::unordered_map<int,int> counts;

    for (const auto& report : reports)
    {
        counts[
            report.tested_block_size
        ]++;
    }

    int best_block = 0;
    int best_count = 0;

    for (const auto& pair : counts)
    {
        if (pair.second > best_count)
        {
            best_count = pair.second;
            best_block = pair.first;
        }
    }

    return best_block;
}
