#include "../include/benchmark_runner.hpp"

#include "../include/console_reporter.hpp"
#include "../include/json_exporter.hpp"
#include "../include/csv_exporter.hpp"

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <cuda_runtime.h>

#include "../kernels/vector_add.hpp"
#include "../kernels/reduction.hpp"
#include "../device/cuda_profiler.hpp"

#include "../include/cuda_check.hpp"

#include "../device/nsight_profiler.hpp"

#include "../include/kernel_classifier.hpp"

static void populate_device_data(
    KernelReport& report,
    const cudaDeviceProp& prop
)
{
    report.gpu_name =
        prop.name;

    report.gpu_uuid =
        "";

    report.compute_capability =
        std::to_string(prop.major)
        + "."
        + std::to_string(prop.minor);

    report.global_memory_mb =
        prop.totalGlobalMem
        / (1024 * 1024);
}

static void print_rankings(
    const std::string& title,
    std::vector<KernelReport>& reports
)
{
    std::sort(
        reports.begin(),
        reports.end(),
        [](
            const KernelReport& a,
            const KernelReport& b
        )
        {
            return
                a.runtime_ms <
                b.runtime_ms;
        }
    );

    std::cout
        << "\n========================================\n";

    std::cout
        << title
        << "\n";

    std::cout
        << "========================================\n\n";

    for (
        std::size_t i = 0;
        i < reports.size();
        ++i
    )
    {
        const auto& r =
            reports[i];

        std::cout
            << "#"
            << (i + 1)
            << " Block "
            << r.tested_block_size
            << " Runtime "
            << r.runtime_ms
            << " ms"
            << " Occupancy "
            << (r.occupancy * 100.0f)
            << "%"
            << "\n";
    }
}

static void populate_profile_data(
    KernelReport& report,
    const ProfilerResult& profile
)
{
    report.occupancy =
        profile.occupancy;

    report.active_blocks_per_sm =
        profile.active_blocks_per_sm;

    report.recommended_block_size =
        profile.recommended_block_size;

    report.recommended_grid_size =
        profile.recommended_grid_size;

    report.registers_per_thread =
        profile.registers_per_thread;

    report.shared_memory_bytes =
        profile.shared_memory_bytes;
}

static void populate_nsight_data(
    KernelReport& report,
    const NsightMetrics& metrics
)
{
    report.dram_throughput_pct =
        metrics.dram_throughput_pct;

    report.compute_memory_throughput_pct =
        metrics.compute_memory_throughput_pct;

    report.nsight_kernel_time_ms =
        metrics.gpu_time_ns / 1e6;

    report.max_warps_active_pct =
        metrics.max_warps_pct;

    report.warps_active_pct =
        metrics.warps_active_pct;

	report.sm_throughput_pct =
    	metrics.sm_throughput_pct;

    report.inst_executed_per_cycle_active_pct =
        metrics.inst_executed_per_cycle_active_pct;

    report.stall_math_pipe_pct =
        metrics.stall_math_pipe_pct;

    report.stall_mio_pct =
        metrics.stall_mio_pct;

    report.stall_tex_pipe_pct =
        metrics.stall_tex_pipe_pct;

    report.inst_fp32 =
        metrics.inst_fp32;

    report.inst_fp64 =
        metrics.inst_fp64;

    report.inst_integer =
        metrics.inst_integer;

    report.inst_memory =
        metrics.inst_memory;

    report.l1_hit_rate_pct =
        metrics.l1_hit_rate_pct;

    report.l2_hit_rate_pct =
        metrics.l2_hit_rate_pct;

    report.l1_global_load_requests =
    metrics.l1_global_load_requests;

    report.l1_global_store_requests =
    metrics.l1_global_store_requests;

    report.l2_sector_average =
    metrics.l2_sector_average;

    report.l2_requests =
    metrics.l2_requests;

    report.l2_hits =
    metrics.l2_hits;

    report.l2_misses =
    metrics.l2_misses;

	report.warps_eligible_pct =
    	metrics.warps_eligible_pct;
	
	report.stall_long_scoreboard_pct =
    	metrics.stall_long_scoreboard_pct;
	
	report.stall_short_scoreboard_pct =
    	metrics.stall_short_scoreboard_pct;
	
	report.stall_not_selected_pct =
    	metrics.stall_not_selected_pct;
	
	report.stall_barrier_pct =
    	metrics.stall_barrier_pct;
}

static void
populate_analysis_report(
    KernelAnalysis& analysis
)
{
    auto& report = analysis.report;
    const auto& state = analysis.state;

    report.kernel_classification =
    analysis.classification.label;

    report.classification_confidence =
    analysis.classification.confidence;

    report.classification_evidence =
    analysis.classification.evidence;

    report.workflow =
    analysis.workflow.type;

    report.memory_bound =
    state.memory_bound;

    report.execution_bound =
    state.execution_bound;

    report.scheduler_bound =
    state.scheduler_bound;

    report.cache_bound =
    state.cache_bound;

    report.execution_pipe_bound =
    state.execution_pipe_bound;

    report.memory_pipeline_bound =
    state.memory_pipeline_bound;

    report.texture_pipeline_bound =
    state.texture_pipeline_bound;

    report.memory_pressure =
    state.memory_pressure;

    report.execution_pressure =
    state.execution_pressure;

    report.scheduler_pressure =
    state.scheduler_pressure;

    report.cache_pressure =
    state.cache_pressure;

    report.utilization_score =
    state.utilization_score;

    report.cache_hit_ratio =
    state.cache_hit_ratio;

    report.fp32_ratio =
    state.fp32_ratio;

    report.integer_ratio =
    state.integer_ratio;

    report.memory_ratio =
    state.memory_ratio;
}

KernelAnalysis
BenchmarkRunner::analyze_kernel(
    KernelReport report
)
{
    KernelAnalysis analysis;

    analysis.report =
    report;

    analysis.state =
    KernelStateBuilder::build(
        analysis.report
    );

    analysis.classification =
    KernelClassifier::classify(
        analysis.state,
        analysis.report
    );

    analysis.optimization =
    KernelOptimizer::analyze(
        analysis.state,
        analysis.report
    );

    analysis.ir =
    KernelIRBuilder::build(
        analysis.report,
        analysis.state,
        analysis.classification,
        analysis.optimization
    );

    analysis.workflow =
    KernelWorkflowBuilder::build(
        analysis.ir
    );

    populate_analysis_report(
        analysis
    );

    return analysis;
}

static std::string current_timestamp()
{
    auto now =
        std::chrono::system_clock::now();

    std::time_t t =
        std::chrono::system_clock::to_time_t(now);

    std::tm tm =
        *std::localtime(&t);

    std::ostringstream oss;

    oss
        << std::put_time(
               &tm,
               "%Y-%m-%d %H:%M:%S"
           );

    return oss.str();
}

KernelReport BenchmarkRunner::run_vector_add(
    int elements,
    int block_size,
    const std::string& session_id
)
{
    KernelReport report;

    std::vector<float> host_a(elements);
    std::vector<float> host_b(elements);
    std::vector<float> host_c(elements);

    for (int i = 0; i < elements; ++i)
    {
        host_a[i] =
            static_cast<float>(i);

        host_b[i] =
            static_cast<float>(i * 2);
    }

    float* device_a = nullptr;
    float* device_b = nullptr;
    float* device_c = nullptr;

    CUDA_CHECK(
        cudaMalloc(
            &device_a,
            elements * sizeof(float)
        )
    );

    CUDA_CHECK(
        cudaMalloc(
            &device_b,
            elements * sizeof(float)
        )
    );

    CUDA_CHECK(
        cudaMalloc(
            &device_c,
            elements * sizeof(float)
        )
    );

    CUDA_CHECK(
        cudaMemcpy(
            device_a,
            host_a.data(),
            elements * sizeof(float),
            cudaMemcpyHostToDevice
        )
    );

    CUDA_CHECK(
        cudaMemcpy(
            device_b,
            host_b.data(),
            elements * sizeof(float),
            cudaMemcpyHostToDevice
        )
    );

    cudaEvent_t start;
    cudaEvent_t stop;

    CUDA_CHECK(
        cudaEventCreate(&start)
    );

    CUDA_CHECK(
        cudaEventCreate(&stop)
    );

    CUDA_CHECK(
        cudaEventRecord(start)
    );

    launch_vector_add(
        device_a,
        device_b,
        device_c,
        elements,
        block_size
    );

    CUDA_CHECK(
        cudaEventRecord(stop)
    );

    CUDA_CHECK(
        cudaEventSynchronize(stop)
    );

    float runtime_ms = 0.0f;

    CUDA_CHECK(
        cudaEventElapsedTime(
            &runtime_ms,
            start,
            stop
        )
    );

    CUDA_CHECK(
        cudaMemcpy(
            host_c.data(),
            device_c,
            elements * sizeof(float),
            cudaMemcpyDeviceToHost
        )
    );

    bool valid = true;

    for (int i = 0; i < elements; ++i)
    {
        float expected =
            host_a[i] + host_b[i];

        if (
            std::fabs(
                host_c[i]
                - expected
            ) > 0.0001f
        )
        {
            valid = false;
            break;
        }
    }

    double bytes_processed =
        static_cast<double>(elements)
        * sizeof(float)
        * 3.0;

    double bandwidth_gb_s =
        bytes_processed
        / runtime_ms
        / 1e6;

    double throughput_gops =
        static_cast<double>(elements)
        / runtime_ms
        / 1e6;

    CudaProfiler profiler;
    
	auto profile =
    	profiler.collect_vector_add(
        	block_size
    	);

    cudaDeviceProp prop;

    CUDA_CHECK(
        cudaGetDeviceProperties(
            &prop,
            0
        )
    );

    report.backend = "cuda";
    report.kernel = "vector_add";

	report.timestamp =
    	current_timestamp();

	report.session_id =
    	session_id;
    
    report.passed = valid;

    report.elements = elements;

    report.runtime_ms =
        runtime_ms;

	report.bandwidth_gb_s =
    	bandwidth_gb_s;
	
	report.throughput_gops =
    	throughput_gops;
	
	report.bytes_processed =
    	bytes_processed;
	
	populate_profile_data(
    	report,
    	profile
	);
	
	report.efficiency_score =
    	report.bandwidth_gb_s
    	*
    	report.occupancy;

    report.tested_block_size =
        block_size;

    report.tested_grid_size =
        (
            elements
            + block_size
            - 1
        )
        / block_size;

	populate_device_data(
    	report,
    	prop
	);

    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_c);

    return report;
}

KernelReport BenchmarkRunner::run_reduction(
    int elements,
    int block_size,
    const std::string& session_id
)
{
    KernelReport report;

    std::vector<float> host_input(
        elements,
        1.0f
    );

    float* device_input =
        nullptr;

    CUDA_CHECK(
        cudaMalloc(
            &device_input,
            elements
            * sizeof(float)
        )
    );

    CUDA_CHECK(
        cudaMemcpy(
            device_input,
            host_input.data(),
            elements
            * sizeof(float),
            cudaMemcpyHostToDevice
        )
    );

    cudaEvent_t start;
    cudaEvent_t stop;

    cudaEventCreate(
        &start
    );

    cudaEventCreate(
        &stop
    );

    cudaEventRecord(
        start
    );

    float reduction_result =
        launch_reduction(
            device_input,
            elements,
            block_size
        );

    cudaEventRecord(
        stop
    );

    cudaEventSynchronize(
        stop
    );

    float runtime_ms =
        0.0f;

    cudaEventElapsedTime(
        &runtime_ms,
        start,
        stop
    );

    float expected =
        static_cast<float>(
            elements
        );

    bool valid =
        std::fabs(
            reduction_result
            - expected
        )
        <
        0.001f;

	if (!valid)
	{
    	std::cout
        	<< "Expected: "
        	<< expected
        	<< "\n";
	
    	std::cout
        	<< "Actual: "
        	<< reduction_result
        	<< "\n";
	}

    CudaProfiler profiler;
    
	auto profile =
    	profiler.collect_reduction(
        	block_size
    	);

	cudaDeviceProp prop;
	
	CUDA_CHECK(
    	cudaGetDeviceProperties(
        	&prop,
        	0
    	)
	);

    report.backend =
        "cuda";

    report.kernel =
        "reduction";

	report.timestamp =
    	current_timestamp();

	report.session_id =
    	session_id;
    
    report.passed =
        valid;

    report.elements =
        elements;

    report.runtime_ms =
        runtime_ms;

    report.tested_block_size =
        block_size;

    report.tested_grid_size =
        (
            elements
            + block_size
            - 1
        )
        /
        block_size;

	populate_device_data(
    	report,
    	prop
	);

	populate_profile_data(
    	report,
    	profile
	);

	report.efficiency_score =
    	(1.0 / report.runtime_ms)
    	* report.occupancy;

    cudaFree(
        device_input
    );

    return report;
}

KernelReport
BenchmarkRunner::sweep_vector_add(
    int elements,
    const std::vector<int>& launch_configs,
    const std::string& session_id
)
{
    KernelReport best;

    bool first = true;
    
    std::vector<KernelReport> sweep_results;

    std::cout
        << "========================================\n";

    std::cout
        << "VECTOR ADD SWEEP\n";

    std::cout
        << "========================================\n\n";

    for (
        int block_size
        : launch_configs
    )
    {
		auto report =
    		run_vector_add(
        		elements,
        		block_size,
        		session_id
    		);

		if (report.passed)
		{
    		sweep_results.push_back(
        		report
    		);
		}

		auto analysis =
		analyze_kernel(report);

        export_csv_row(
            report,
            analysis.state,
            analysis.classification,
            analysis.workflow,
            "results/vector_add/latest_sweep.csv"
        );

        export_csv_row(
            report,
            analysis.state,
            analysis.classification,
            analysis.workflow,
            "results/vector_add/full_history.csv"
        );

        export_json(
            report,
            analysis.state,
            analysis.classification,
            analysis.optimization,
            analysis.ir,
            analysis.workflow,
            "results/vector_add/"
            +
            std::to_string(block_size)
            +
            ".json"
        );

        ConsoleReporter::print(
            report
        );

		if (
    		report.passed &&
    		(
        		first ||
        		report.runtime_ms <
        		best.runtime_ms
    		)
		)
        {
            best =
                report;

            first =
                false;
        }
    }

	std::sort(
    	sweep_results.begin(),
    	sweep_results.end(),
    	[](
        	const KernelReport& a,
        	const KernelReport& b
    	)
    	{
        	return
            	a.runtime_ms <
            	b.runtime_ms;
    	}
	);
	
	last_vector_add_results =
    	sweep_results;
    
	print_rankings(
    	"VECTOR ADD RANKINGS",
    	sweep_results
	);

    	
    auto best_analysis =
    analyze_kernel(best);

    export_json(
        best,
        best_analysis.state,
        best_analysis.classification,
        best_analysis.optimization,
        best_analysis.ir,
        best_analysis.workflow,
        "results/vector_add/best.json"
    );

    return best;
}

KernelReport
BenchmarkRunner::sweep_reduction(
    int elements,
    const std::vector<int>& launch_configs,
    const std::string& session_id
)
{
    KernelReport best;

    bool first = true;

	std::vector<KernelReport> sweep_results;

    std::cout
        << "========================================\n";

    std::cout
        << "REDUCTION SWEEP\n";

    std::cout
        << "========================================\n\n";

    for (
        int block_size
        : launch_configs
    )
    {
		auto report =
    		run_reduction(
        		elements,
        		block_size,
        		session_id
    		);

		if (report.passed)
		{
    		sweep_results.push_back(
        		report
    		);
		}

		auto analysis =
		analyze_kernel(report);

		export_csv_row(
            report,
            analysis.state,
            analysis.classification,
            analysis.workflow,
            "results/reduction/latest_sweep.csv"
        );

        export_csv_row(
            report,
            analysis.state,
            analysis.classification,
            analysis.workflow,
            "results/reduction/full_history.csv"
        );

        export_json(
            report,
            analysis.state,
            analysis.classification,
            analysis.optimization,
            analysis.ir,
            analysis.workflow,
            "results/reduction/"
            +
            std::to_string(block_size)
            +
            ".json"
        );

        ConsoleReporter::print(
            report
        );

		if (
    		report.passed &&
    		(
        		first ||
        		report.runtime_ms <
        		best.runtime_ms
    		)
		)
        {
            best =
                report;

            first =
                false;
        }
    }

	std::sort(
    	sweep_results.begin(),
    	sweep_results.end(),
    	[](
        	const KernelReport& a,
        	const KernelReport& b
    	)
    	{
        	return
            	a.runtime_ms <
            	b.runtime_ms;
    	}
	);
	
	last_reduction_results =
    	sweep_results;
    
	print_rankings(
    	"REDUCTION RANKINGS",
    	sweep_results
	);
	
    auto best_analysis =
    analyze_kernel(best);

    export_json(
        best,
        best_analysis.state,
        best_analysis.classification,
        best_analysis.optimization,
        best_analysis.ir,
        best_analysis.workflow,
        "results/reduction/best.json"
    );

    return best;
}

KernelAnalysis BenchmarkRunner::profile_vector_add_winner(
    KernelReport& report
)
{
    NsightProfiler profiler;

    auto metrics =
        profiler.profile_vector_add(
            report.tested_block_size
        );

    populate_nsight_data(
        report,
        metrics
    );

    report.has_nsight_metrics =
    metrics.valid;

    return analyze_kernel(report);
}

KernelAnalysis BenchmarkRunner::profile_reduction_winner(
    KernelReport& report
)
{
    NsightProfiler profiler;

    auto metrics =
        profiler.profile_reduction(
            report.tested_block_size
        );

    populate_nsight_data(
        report,
        metrics
    );

    report.has_nsight_metrics =
    metrics.valid;

    return analyze_kernel(report);
}

KernelReport
BenchmarkRunner::vector_add_runner_up() const
{
    if (
        last_vector_add_results.size()
        < 2
    )
    {
        return {};
    }

    return
        last_vector_add_results[1];
}

KernelReport
BenchmarkRunner::reduction_runner_up() const
{
    if (
        last_reduction_results.size()
        < 2
    )
    {
        return {};
    }

    return
        last_reduction_results[1];
}

KernelAnalysis
BenchmarkRunner::profile_vector_add_runner_up(
    KernelReport& report
)
{
    NsightProfiler profiler;

    auto metrics =
        profiler.profile_vector_add(
            report.tested_block_size
        );

    populate_nsight_data(
        report,
        metrics
    );

    report.has_nsight_metrics =
    metrics.valid;

    return analyze_kernel(report);
}

KernelAnalysis
BenchmarkRunner::profile_reduction_runner_up(
    KernelReport& report
)
{
    NsightProfiler profiler;

    auto metrics =
        profiler.profile_reduction(
            report.tested_block_size
        );

    populate_nsight_data(
        report,
        metrics
    );

    report.has_nsight_metrics =
    metrics.valid;

    return analyze_kernel(report);
}
