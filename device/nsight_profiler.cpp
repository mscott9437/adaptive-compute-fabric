#include "nsight_profiler.hpp"

#include <array>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

std::string
NsightProfiler::run_command(
    const std::string& command
)
{
    std::array<char, 4096> buffer;

    std::string output;

	std::string full_command =
    	command + " 2>&1";
	
	FILE* pipe =
    	popen(
        	full_command.c_str(),
        	"r"
    	);

    if (!pipe)
    {
        return "";
    }

    while (
        fgets(
            buffer.data(),
            buffer.size(),
            pipe
        ) != nullptr
    )
    {
        output +=
            buffer.data();
    }

    pclose(pipe);

    return output;
}

static std::vector<std::string>
split_csv_row(
    const std::string& line
)
{
    std::vector<std::string> result;

    std::string current;

    bool quoted = false;

    for (
        char c : line
    )
    {
        if (
            c == '"'
        )
        {
            quoted = !quoted;
            continue;
        }

        if (
            c == ',' &&
            !quoted
        )
        {
            result.push_back(
                current
            );

            current.clear();

            continue;
        }

        current += c;
    }

    result.push_back(
        current
    );

    return result;
}

static std::string
remove_commas(
    std::string value
)
{
    value.erase(
        std::remove(
            value.begin(),
            value.end(),
            ','
        ),
        value.end()
    );

    return value;
}

NsightMetrics
NsightProfiler::parse_csv(
    const std::string& csv_output
)
{
	NsightMetrics metrics;
	
	metrics.dram_throughput_pct = 0.0;
	metrics.compute_memory_throughput_pct = 0.0;
	
	metrics.gpu_time_ns = 0.0;
	
	metrics.max_warps_pct = 0.0;
	
	metrics.sm_throughput_pct = 0.0;
	
	metrics.warps_active_pct = 0.0;
	
	metrics.warps_eligible_pct = 0.0;
	
	metrics.stall_long_scoreboard_pct = 0.0;
	
	metrics.stall_short_scoreboard_pct = 0.0;
	
	metrics.stall_not_selected_pct = 0.0;
	
	metrics.stall_barrier_pct = 0.0;

    std::stringstream stream(
        csv_output
    );

    std::string line;

    std::vector<std::string> header;

    bool found_header =
        false;

    std::size_t dram_index =
        std::string::npos;

    std::size_t compute_index =
        std::string::npos;

    std::size_t gpu_time_index =
        std::string::npos;

    std::size_t max_warps_index =
        std::string::npos;

	std::size_t sm_throughput_index =
    	std::string::npos;

	std::size_t warps_active_index =
    	std::string::npos;
    
	std::size_t warps_eligible_index =
    	std::string::npos;
	
	std::size_t stall_long_index =
    	std::string::npos;
	
	std::size_t stall_short_index =
    	std::string::npos;
	
	std::size_t stall_not_selected_index =
    	std::string::npos;
	
	std::size_t stall_barrier_index =
    	std::string::npos;
    
    while (
        std::getline(
            stream,
            line
        )
    )
    {
        if (
            !found_header &&
            line.find("\"ID\"")
                != std::string::npos
        )
        {
            header =
                split_csv_row(
                    line
                );

            for (
                std::size_t i = 0;
                i < header.size();
                ++i
            )
            {
                if (
                    header[i] ==
                    "gpu__dram_throughput.avg.pct_of_peak_sustained_elapsed"
                )
                {
                    dram_index = i;
                }

                if (
                    header[i] ==
                    "gpu__compute_memory_throughput.avg.pct_of_peak_sustained_elapsed"
                )
                {
                    compute_index = i;
                }

                if (
                    header[i] ==
                    "gpu__time_duration.sum"
                )
                {
                    gpu_time_index = i;
                }

                if (
                    header[i] ==
                    "sm__maximum_warps_per_active_cycle_pct"
                )
                {
                    max_warps_index = i;
                }
                
                if (
    				header[i] ==
    				"sm__throughput.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				sm_throughput_index = i;
				}

				if (
    				header[i] ==
    				"smsp__warps_active.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				warps_active_index = i;
				}

				if (
    				header[i] ==
    				"smsp__warps_eligible.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				warps_eligible_index = i;
				}
				
				if (
    				header[i] ==
    				"smsp__warps_issue_stalled_long_scoreboard.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				stall_long_index = i;
				}
				
				if (
    				header[i] ==
    				"smsp__warps_issue_stalled_short_scoreboard.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				stall_short_index = i;
				}
				
				if (
    				header[i] ==
    				"smsp__warps_issue_stalled_not_selected.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				stall_not_selected_index = i;
				}
				
				if (
    				header[i] ==
    				"smsp__warps_issue_stalled_barrier.avg.pct_of_peak_sustained_elapsed"
				)
				{
    				stall_barrier_index = i;
				}
            }

            found_header =
                true;

            continue;
        }

        if (!found_header)
        {
            continue;
        }

        if (
            line.empty() ||
            line[0] != '"'
        )
        {
            continue;
        }

        auto row =
            split_csv_row(
                line
            );

        try
        {
            if (
                dram_index != std::string::npos &&
                dram_index < row.size()
            )
            {
                metrics.dram_throughput_pct =
                    std::stod(
                        remove_commas(
                            row[dram_index]
                        )
                    );
            }

            if (
                compute_index != std::string::npos &&
                compute_index < row.size()
            )
            {
                metrics.compute_memory_throughput_pct =
                    std::stod(
                        remove_commas(
                            row[compute_index]
                        )
                    );
            }

            if (
                gpu_time_index != std::string::npos &&
                gpu_time_index < row.size()
            )
            {
                metrics.gpu_time_ns =
                    std::stod(
                        remove_commas(
                            row[gpu_time_index]
                        )
                    );
            }

            if (
                max_warps_index != std::string::npos &&
                max_warps_index < row.size()
            )
            {
                metrics.max_warps_pct =
                    std::stod(
                        remove_commas(
                            row[max_warps_index]
                        )
                    );
            }

			if (
    			sm_throughput_index != std::string::npos &&
    			sm_throughput_index < row.size()
			)
			{
    			metrics.sm_throughput_pct =
        			std::stod(
            			remove_commas(
                			row[sm_throughput_index]
            			)
        			);
			}

			if (
    			warps_active_index != std::string::npos &&
    			warps_active_index < row.size()
			)
			{
    			metrics.warps_active_pct =
        			std::stod(
            			remove_commas(
                			row[warps_active_index]
            			)
        			);
			}

			if (
    			warps_eligible_index != std::string::npos &&
    			warps_eligible_index < row.size()
			)
			{
    			metrics.warps_eligible_pct =
        			std::stod(
            			remove_commas(
                			row[warps_eligible_index]
            			)
        			);
			}
			
			if (
    			stall_long_index != std::string::npos &&
    			stall_long_index < row.size()
			)
			{
    			metrics.stall_long_scoreboard_pct =
        			std::stod(
            			remove_commas(
                			row[stall_long_index]
            			)
        			);
			}
			
			if (
    			stall_short_index != std::string::npos &&
    			stall_short_index < row.size()
			)
			{
    			metrics.stall_short_scoreboard_pct =
        			std::stod(
            			remove_commas(
                			row[stall_short_index]
            			)
        			);
			}
			
			if (
    			stall_not_selected_index != std::string::npos &&
    			stall_not_selected_index < row.size()
			)
			{
    			metrics.stall_not_selected_pct =
        			std::stod(
            			remove_commas(
                			row[stall_not_selected_index]
            			)
        			);
			}
			
			if (
    			stall_barrier_index != std::string::npos &&
    			stall_barrier_index < row.size()
			)
			{
    			metrics.stall_barrier_pct =
        			std::stod(
            			remove_commas(
                			row[stall_barrier_index]
            			)
        			);
			}

            metrics.valid = true;

            break;
        }
        catch (...)
        {
        }
    }

    return metrics;
}

NsightMetrics
NsightProfiler::run_profile_command(
    const std::string& target
)
{
    std::stringstream command;

    command
        << "sudo ncu "
        << "--csv "
        << "--page raw "
        << "--metrics "
		<< "gpu__dram_throughput.avg.pct_of_peak_sustained_elapsed,"
		<< "gpu__compute_memory_throughput.avg.pct_of_peak_sustained_elapsed,"
		<< "gpu__time_duration.sum,"
		<< "sm__maximum_warps_per_active_cycle_pct,"
		<< "sm__throughput.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_active.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_eligible.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_issue_stalled_long_scoreboard.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_issue_stalled_short_scoreboard.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_issue_stalled_not_selected.avg.pct_of_peak_sustained_elapsed,"
		<< "smsp__warps_issue_stalled_barrier.avg.pct_of_peak_sustained_elapsed "
        << target;

    return parse_csv(
        run_command(
            command.str()
        )
    );
}

NsightMetrics
NsightProfiler::profile_vector_add(
    int block_size
)
{
    return run_profile_command(
        "./compute_fabric --profile-vector-add "
        + std::to_string(block_size)
    );
}

NsightMetrics
NsightProfiler::profile_reduction(
    int block_size
)
{
    return run_profile_command(
        "./compute_fabric --profile-reduction "
        + std::to_string(block_size)
    );
}
