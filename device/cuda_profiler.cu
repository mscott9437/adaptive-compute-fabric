#include "cuda_profiler.hpp"

#include <iostream>

#include "../kernels/vector_add.hpp"
#include "../kernels/reduction.hpp"

#include <cuda_runtime.h>

ProfilerResult
CudaProfiler::collect_vector_add(
    int tested_block_size
)
{
    ProfilerResult result;

    result.backend = "cuda";
    result.kernel = "vector_add";

    int min_grid_size = 0;
    int block_size = 0;

    cudaOccupancyMaxPotentialBlockSize(
        &min_grid_size,
        &block_size,
        vector_add_kernel,
        0,
        0
    );

    result.recommended_grid_size =
        min_grid_size;

    result.recommended_block_size =
        block_size;

    int active_blocks = 0;

	cudaOccupancyMaxActiveBlocksPerMultiprocessor(
    	&active_blocks,
    	vector_add_kernel,
    	tested_block_size,
    	0
	);

	result.active_blocks_per_sm =
    	active_blocks;
    
    cudaDeviceProp prop;

    cudaGetDeviceProperties(
        &prop,
        0
    );

    result.occupancy =
        (
			active_blocks *
			tested_block_size
        )
        /
        static_cast<float>(
            prop.maxThreadsPerMultiProcessor
        );

	cudaFuncAttributes attrs;
	
	cudaFuncGetAttributes(
    	&attrs,
    	vector_add_kernel
	);


	result.registers_per_thread =
    	attrs.numRegs;
	
	result.shared_memory_bytes =
    	attrs.sharedSizeBytes;

    return result;
}

ProfilerResult
CudaProfiler::collect_reduction(
    int tested_block_size
)
{
    ProfilerResult result;

    result.backend = "cuda";
    result.kernel = "reduction";

    int min_grid_size = 0;
    int block_size = 0;

    cudaOccupancyMaxPotentialBlockSize(
        &min_grid_size,
        &block_size,
        reduction_kernel,
        0,
        0
    );

    result.recommended_grid_size =
        min_grid_size;

    result.recommended_block_size =
        block_size;

    int active_blocks = 0;

	cudaOccupancyMaxActiveBlocksPerMultiprocessor(
    	&active_blocks,
    	reduction_kernel,
    	tested_block_size,
    	0
	);

	result.active_blocks_per_sm =
    	active_blocks;

    cudaDeviceProp prop;

    cudaGetDeviceProperties(
        &prop,
        0
    );

    result.occupancy =
    (
		active_blocks *
		tested_block_size
    )
    /
    static_cast<float>(
        prop.maxThreadsPerMultiProcessor
    );

    cudaFuncAttributes attrs;

    cudaFuncGetAttributes(
        &attrs,
        reduction_kernel
    );

    result.registers_per_thread =
        attrs.numRegs;

    result.shared_memory_bytes =
        attrs.sharedSizeBytes;

    return result;
}
