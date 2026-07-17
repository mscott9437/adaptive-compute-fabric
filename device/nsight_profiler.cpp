#include "nsight_profiler.hpp"

#include <array>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include <fstream>

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

    metrics.inst_executed_per_cycle_active_pct = 0.0;

    metrics.stall_math_pipe_pct = 0.0;

    metrics.stall_mio_pct = 0.0;

    metrics.stall_tex_pipe_pct = 0.0;

    metrics.inst_fp32 = 0.0;

    metrics.inst_fp64 = 0.0;

    metrics.inst_integer = 0.0;

    metrics.inst_memory = 0.0;

    metrics.l1_hit_rate_pct = 0.0;

    metrics.l2_hit_rate_pct = 0.0;

    metrics.l1_throughput_pct = 0.0;

    metrics.l2_throughput_pct = 0.0;

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

    std::size_t inst_per_cycle_index =
    std::string::npos;

    std::size_t math_pipe_index =
    std::string::npos;

    std::size_t mio_index =
    std::string::npos;

    std::size_t tex_index =
    std::string::npos;

    std::size_t fp32_index =
    std::string::npos;

    std::size_t fp64_index =
    std::string::npos;

    std::size_t integer_index =
    std::string::npos;

    std::size_t memory_index =
    std::string::npos;

    std::size_t l1_hit_index =
    std::string::npos;

    std::size_t l2_hit_index =
    std::string::npos;

    std::size_t l1_throughput_index =
    std::string::npos;

    std::size_t l2_throughput_index =
    std::string::npos;

    std::size_t l1_load_requests_index = std::string::npos;
    std::size_t l1_store_requests_index = std::string::npos;

    std::size_t l2_sector_average_index = std::string::npos;
    std::size_t l2_requests_index = std::string::npos;
    std::size_t l2_hits_index = std::string::npos;
    std::size_t l2_misses_index = std::string::npos;

    while (
        std::getline(
            stream,
            line
        )
    )
    {
        if(
            !found_header &&
            (
                line.find("gpu__") != std::string::npos ||
                line.find("sm__") != std::string::npos ||
                line.find("smsp__") != std::string::npos ||
                line.find("l1tex__") != std::string::npos ||
                line.find("lts__") != std::string::npos
            )
        )
        {
            header =
            split_csv_row(
                line
            );

            auto find_metric =
            [&](
                const char* metric
            )
            {
                for(std::size_t i = 0; i < header.size(); ++i)
                {
                    if(header[i].find(metric) != std::string::npos)
                    {
                        return i;
                    }
                }

                return std::string::npos;
            };

            dram_index =
            find_metric(
                "gpu__dram_throughput.avg.pct_of_peak_sustained_elapsed"
            );

            compute_index =
            find_metric(
                "gpu__compute_memory_throughput.avg.pct_of_peak_sustained_elapsed"
            );

            gpu_time_index =
            find_metric(
                "gpu__time_duration.sum"
            );

            max_warps_index =
            find_metric(
                "sm__maximum_warps_per_active_cycle_pct"
            );

            sm_throughput_index =
            find_metric(
                "sm__throughput.avg.pct_of_peak_sustained_elapsed"
            );

            warps_active_index =
            find_metric(
                "smsp__warps_active.avg.pct_of_peak_sustained_elapsed"
            );

            warps_eligible_index =
            find_metric(
                "smsp__warps_eligible.avg.pct_of_peak_sustained_elapsed"
            );

            stall_long_index =
            find_metric(
                "smsp__warps_issue_stalled_long_scoreboard"
            );

            stall_short_index =
            find_metric(
                "smsp__warps_issue_stalled_short_scoreboard"
            );

            stall_not_selected_index =
            find_metric(
                "smsp__warps_issue_stalled_not_selected"
            );

            stall_barrier_index =
            find_metric(
                "smsp__warps_issue_stalled_barrier"
            );

            inst_per_cycle_index =
            find_metric(
                "smsp__inst_executed.avg.per_cycle_active"
            );

            math_pipe_index =
            find_metric(
                "smsp__warps_issue_stalled_math_pipe_throttle"
            );

            mio_index =
            find_metric(
                "smsp__warps_issue_stalled_mio_throttle"
            );

            tex_index =
            find_metric(
                "smsp__warps_issue_stalled_tex_throttle"
            );

            fp32_index =
            find_metric(
                "smsp__sass_thread_inst_executed_op_fp32"
            );

            fp64_index =
            find_metric(
                "smsp__sass_thread_inst_executed_op_fp64"
            );

            integer_index =
            find_metric(
                "smsp__sass_thread_inst_executed_op_integer"
            );

            memory_index =
            find_metric(
                "smsp__sass_thread_inst_executed_op_memory"
            );

            l1_hit_index =
            find_metric(
                "l1tex__t_sector_hit_rate"
            );

            l2_hit_index =
            find_metric(
                "lts__t_sector_hit_rate"
            );

            l1_load_requests_index =
            find_metric(
                "l1tex__t_requests_pipe_lsu_mem_global_op_ld"
            );

            l1_store_requests_index =
            find_metric(
                "l1tex__t_requests_pipe_lsu_mem_global_op_st"
            );

            l2_sector_average_index =
            find_metric(
                "lts__t_sectors.avg"
            );

            l2_requests_index =
            find_metric(
                "lts__t_requests.sum"
            );

            l2_hits_index =
            find_metric(
                "lts__t_sectors_hit.sum"
            );

            l2_misses_index =
            find_metric(
                "lts__t_sectors_miss.sum"
            );

            found_header =
                true;

            continue;
        }

        if (!found_header)
        {
            continue;
        }

        if(line.empty())
        {
            continue;
        }

        auto row =
            split_csv_row(
                line
            );

            if(row.empty())
            {
                continue;
            }

            //
            // Skip the units row.
            // It always has an empty Process ID.
            //
            if(row.size() > 1 && row[1].empty())
            {
                continue;
            }

            //
            // Skip anything before the actual kernel record.
            //
            if(row[0] != "0")
            {
                continue;
            }

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

			if(
                inst_per_cycle_index != std::string::npos &&
                inst_per_cycle_index < row.size()
            )
            {
                metrics.inst_executed_per_cycle_active_pct =
                std::stod(
                    remove_commas(
                        row[inst_per_cycle_index]
                    )
                );
            }

            if (
                math_pipe_index != std::string::npos &&
                math_pipe_index < row.size()
            )
            {
                metrics.stall_math_pipe_pct =
                std::stod(remove_commas(row[math_pipe_index]));
            }

            if (
                mio_index != std::string::npos &&
                mio_index < row.size()
            )
            {
                metrics.stall_mio_pct =
                std::stod(remove_commas(row[mio_index]));
            }

            if (
                tex_index != std::string::npos &&
                tex_index < row.size()
            )
            {
                metrics.stall_tex_pipe_pct =
                std::stod(remove_commas(row[tex_index]));
            }

            if (
                fp32_index != std::string::npos &&
                fp32_index < row.size()
            )
            {
                metrics.inst_fp32 =
                std::stod(remove_commas(row[fp32_index]));
            }

            if (
                fp64_index != std::string::npos &&
                fp64_index < row.size()
            )
            {
                metrics.inst_fp64 =
                std::stod(remove_commas(row[fp64_index]));
            }

            if (
                integer_index != std::string::npos &&
                integer_index < row.size()
            )
            {
                metrics.inst_integer =
                std::stod(remove_commas(row[integer_index]));
            }

            if (
                memory_index != std::string::npos &&
                memory_index < row.size()
            )
            {
                metrics.inst_memory =
                std::stod(remove_commas(row[memory_index]));
            }

            if (
                l1_hit_index != std::string::npos &&
                l1_hit_index < row.size()
            )
            {
                metrics.l1_hit_rate_pct =
                std::stod(remove_commas(row[l1_hit_index]));
            }

            if (
                l2_hit_index != std::string::npos &&
                l2_hit_index < row.size()
            )
            {
                metrics.l2_hit_rate_pct =
                std::stod(remove_commas(row[l2_hit_index]));
            }

            if (
                l1_load_requests_index != std::string::npos &&
                l1_load_requests_index < row.size()
            )
            {
                metrics.l1_global_load_requests =
                std::stod(remove_commas(row[l1_load_requests_index]));
            }

            if (
                l1_store_requests_index != std::string::npos &&
                l1_store_requests_index < row.size()
            )
            {
                metrics.l1_global_store_requests =
                std::stod(remove_commas(row[l1_store_requests_index]));
            }

            if (
                l2_sector_average_index != std::string::npos &&
                l2_sector_average_index < row.size()
            )
            {
                metrics.l2_sector_average =
                std::stod(remove_commas(row[l2_sector_average_index]));
            }

            if (
                l2_requests_index != std::string::npos &&
                l2_requests_index < row.size()
            )
            {
                metrics.l2_requests =
                std::stod(remove_commas(row[l2_requests_index]));
            }

            if (
                l2_hits_index != std::string::npos &&
                l2_hits_index < row.size()
            )
            {
                metrics.l2_hits =
                std::stod(remove_commas(row[l2_hits_index]));
            }

            if (
                l2_misses_index != std::string::npos &&
                l2_misses_index < row.size()
            )
            {
                metrics.l2_misses =
                std::stod(remove_commas(row[l2_misses_index]));
            }

            break;
        }
        catch(const std::exception& e)
        {
            std::cout
            << "\nParse exception: "
            << e.what()
            << "\n";

            std::cout
            << "Row:\n";

            for(std::size_t i = 0; i < row.size(); ++i)
            {
                std::cout
                << i
                << " = ["
                << row[i]
                << "]\n";
            }
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
		<< "smsp__warps_issue_stalled_barrier.avg.pct_of_peak_sustained_elapsed,"
        << "smsp__inst_executed.avg.per_cycle_active,"
        << "smsp__warps_issue_stalled_math_pipe_throttle.avg.pct_of_peak_sustained_elapsed,"
        << "smsp__warps_issue_stalled_mio_throttle.avg.pct_of_peak_sustained_elapsed,"
        << "smsp__warps_issue_stalled_tex_throttle.avg.pct_of_peak_sustained_elapsed,"
        << "smsp__sass_thread_inst_executed_op_fp32.sum,"
        << "smsp__sass_thread_inst_executed_op_fp64.sum,"
        << "smsp__sass_thread_inst_executed_op_integer.sum,"
        << "smsp__sass_thread_inst_executed_op_memory.sum,"
        << "l1tex__t_sector_hit_rate.pct,"
        << "l1tex__t_requests_pipe_lsu_mem_global_op_ld.sum,"
        << "l1tex__t_requests_pipe_lsu_mem_global_op_st.sum,"
        << "lts__t_sector_hit_rate.pct,"
        << "lts__t_sectors.avg,"
        << "lts__t_requests.sum,"
        << "lts__t_sectors_hit.sum,"
        << "lts__t_sectors_miss.sum"
        << " -- "
        << target;

        std::string csv =
        run_command(
            command.str()
        );

        std::ofstream dump(
            "results/nsight_raw.csv"
        );

        dump << csv;

        dump.close();

        return parse_csv(csv);
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
