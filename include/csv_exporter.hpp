#pragma once

#include "kernel_report.hpp"

#include <string>

void export_csv_row(
    const KernelReport& report,
    const std::string& path
);
