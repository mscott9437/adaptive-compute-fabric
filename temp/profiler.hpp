#pragma once

#include "profiler_result.hpp"

class Profiler
{
public:

    virtual ~Profiler() = default;

    virtual ProfilerResult
    collect() = 0;
};
