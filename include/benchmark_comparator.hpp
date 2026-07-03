#pragma once

#include "kernel_report.hpp"

class BenchmarkComparator
{
public:

    static void compare(
        const KernelReport& previous,
        const KernelReport& current
    );
};
