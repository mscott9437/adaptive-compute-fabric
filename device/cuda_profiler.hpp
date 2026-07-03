#pragma once

#include "../include/profiler_result.hpp"

class CudaProfiler
{
public:

	ProfilerResult
	collect_vector_add(
    	int tested_block_size
	);

	ProfilerResult
	collect_reduction(
    	int tested_block_size
	);
};
