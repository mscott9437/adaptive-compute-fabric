#pragma once

#include <string>

#include "kernel_report.hpp"

struct KernelState;
struct KernelClassification;
struct KernelWorkflow;

void export_csv_row(
    const KernelReport& report,
    const KernelState& state,
    const KernelClassification& classification,
    const KernelWorkflow& workflow,
    const std::string& path
);
