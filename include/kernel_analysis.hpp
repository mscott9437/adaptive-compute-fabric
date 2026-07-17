#pragma once

#include "kernel_report.hpp"
#include "kernel_state.hpp"
#include "kernel_classifier.hpp"
#include "kernel_optimizer.hpp"
#include "kernel_ir.hpp"
#include "kernel_workflow.hpp"

struct KernelAnalysis
{
    KernelReport report;

    KernelState state;

    KernelClassification classification;

    KernelOptimization optimization;

    KernelIR ir;

    KernelWorkflow workflow;
};
