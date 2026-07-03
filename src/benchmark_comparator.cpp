#include "../include/benchmark_comparator.hpp"

#include <iostream>

void BenchmarkComparator::compare(
    const KernelReport& previous,
    const KernelReport& current
)
{
    double delta =
        (
            current.runtime_ms
            -
            previous.runtime_ms
        )
        /
        previous.runtime_ms
        * 100.0;

    std::cout
        << "\n========================================\n";

    std::cout
        << "HISTORICAL COMPARISON\n";

    std::cout
        << "========================================\n";

    std::cout
        << "Previous Best Runtime: "
        << previous.runtime_ms
        << " ms\n";

    std::cout
        << "Current Runtime: "
        << current.runtime_ms
        << " ms\n";

    if (delta < 0.0)
    {
        std::cout
            << "Improvement: "
            << -delta
            << "%\n";
    }
    else
    {
        std::cout
            << "Regression: "
            << delta
            << "%\n";
    }
}
