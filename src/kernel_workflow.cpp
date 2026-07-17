#include "../include/kernel_workflow.hpp"

KernelWorkflow
KernelWorkflowBuilder::build(
    const KernelIR& ir
)
{
    KernelWorkflow workflow;

    workflow.kernel =
    ir.kernel;

    if(ir.bottleneck == "Memory")
    {
        workflow.type =
        "Streaming Kernel";

    workflow.summary =
    "Bandwidth dominated workload.";
    }
    else
        if(ir.bottleneck == "Execution")
        {
            workflow.type =
            "Dense Compute";

        workflow.summary =
        "Arithmetic intensive kernel.";
        }
        else
            if(ir.bottleneck == "Synchronization")
            {
                workflow.type =
                "Reduction";

        workflow.summary =
        "Synchronization dominated workload.";
            }
            else
            {
                workflow.type =
                "General CUDA Kernel";

                workflow.summary =
                "Mixed execution characteristics.";
            }

            return workflow;
}
