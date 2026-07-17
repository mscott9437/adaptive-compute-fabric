#pragma once

#include <string>
#include <vector>

#include "kernel_report.hpp"

struct KernelExplanation
{
    std::string summary;

    std::vector<std::string> evidence;
};

class KernelExplainer
{
public:

    static KernelExplanation explain(
        const KernelReport& winner,
        const KernelReport& runner_up
    );
};
