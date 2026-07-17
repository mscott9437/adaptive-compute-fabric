#pragma once

#include "kernel_state.hpp"
#include "kernel_report.hpp"

#include <string>
#include <vector>

struct KernelOptimization
{
    std::vector<std::string> recommendations;
};

class KernelOptimizer
{
public:

    static KernelOptimization analyze(
        const KernelState& state,
        const KernelReport& report
    );
};
