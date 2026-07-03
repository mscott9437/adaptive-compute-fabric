#pragma once

#include <string>
#include <vector>

#include "kernel_report.hpp"

struct KernelClassification
{
    std::string label;

    double confidence = 0.0;

    std::vector<std::string> evidence;
};

class KernelClassifier
{
public:

    static KernelClassification classify(
        const KernelReport& report
    );
};
