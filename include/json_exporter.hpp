#pragma once

#include "kernel_report.hpp"

#include <string>

void export_json(
    const KernelReport& report,
    const std::string& path
);
