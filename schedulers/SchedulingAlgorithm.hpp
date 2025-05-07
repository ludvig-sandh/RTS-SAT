#pragma once

#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "Schedule.hpp"

class SchedulingAlgorithm {
public:
    virtual ~SchedulingAlgorithm() = default;

    // Core method each algorithm must implement
    virtual Schedule GenerateSchedule(TaskSet &taskSet) = 0;
};