#pragma once

#include "kernel_report.hpp"
#include "kernel_state.hpp"
#include "kernel_classifier.hpp"
#include "kernel_optimizer.hpp"
#include "kernel_ir.hpp"
#include "kernel_explainer.hpp"
#include "kernel_workflow.hpp"

class ConsoleReporter
{
public:

    static void print(
        const KernelReport& report
    );

    static void print_nsight_summary(
        const KernelReport& report
    );
    
    static void print_winner_comparison(
    	const KernelReport& winner,
    	const KernelReport& runner_up
	);
	
	static void print_explanation(
    	const KernelExplanation& explanation
	);

    static void print_kernel_state(
        const KernelState& state
    );

    static void print_classification(
        const KernelClassification& classification
    );

    static void print_optimizer(
        const KernelOptimization& optimization
    );

    static void print_ir_summary(
        const KernelIR& ir
    );

    static void print_workflow(
        const KernelWorkflow& workflow
    );
};
