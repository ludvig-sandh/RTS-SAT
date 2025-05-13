#pragma once

#include <vector>
#include <stdexcept>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "MultiprocessorSchedule.hpp"

class MultiprocessorSchedulingAlgorithm {
public:
    virtual ~MultiprocessorSchedulingAlgorithm() = default;

    // Core method each algorithm must implement
    MultiprocessorSchedule GenerateSchedule(TaskSet &taskSet, uint32_t numCpus);

protected:
    virtual MultiprocessorSchedule GenerateScheduleImpl(TaskSet& taskSet, uint32_t numCpus);
};