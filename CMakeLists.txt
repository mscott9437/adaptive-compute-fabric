cmake_minimum_required(VERSION 3.24)

project(
    AdaptiveComputeFabric
    LANGUAGES CXX CUDA
)

set(CMAKE_CXX_STANDARD 20)

add_executable(

    compute_fabric

    src/main.cpp

    src/json_exporter.cpp

    src/console_reporter.cpp

    src/benchmark_runner.cpp

    src/csv_exporter.cpp

    src/history_loader.cpp

    src/benchmark_history.cpp

    src/benchmark_comparator.cpp

    src/nsight_capture.cpp

    src/kernel_classifier.cpp

    device/cuda_profiler.cu

    device/nsight_profiler.cpp

    kernels/vector_add.cu

    kernels/reduction.cu
)
