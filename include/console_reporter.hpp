#pragma once

#include "kernel_report.hpp"

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
};
