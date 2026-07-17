#pragma once

#include "kernel_report.hpp"

#include <string>

struct KernelState;
struct KernelClassification;
struct KernelOptimization;
struct KernelIR;
struct KernelWorkflow;

void export_json(
    const KernelReport& report,
    const KernelState& state,
    const KernelClassification& classification,
    const KernelOptimization& optimization,
    const KernelIR& ir,
    const KernelWorkflow& workflow,
    const std::string& path
);
