#pragma once

#include <string>

#include "kernel_ir.hpp"

struct KernelWorkflow
{
    std::string kernel;

    std::string type;

    std::string summary;
};

class KernelWorkflowBuilder
{
public:

    static KernelWorkflow build(
        const KernelIR& ir
    );
};
