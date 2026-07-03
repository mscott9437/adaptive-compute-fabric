#pragma once

#include "kernel_report.hpp"

#include <optional>
#include <string>

class HistoryLoader
{
public:

    static std::optional<KernelReport>
    load_best(
        const std::string& path
    );
};
