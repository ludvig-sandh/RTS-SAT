#pragma once

#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "Schedule.hpp"

struct StaticPrioritiesComparator {
    const TaskSet& taskSet;

    StaticPrioritiesComparator(const TaskSet& ts) : taskSet(ts) {}

    bool operator()(const TaskJob& a, const TaskJob& b) const {
        uint32_t prioA = taskSet.GetTask(a.taskId).prio;
        uint32_t prioB = taskSet.GetTask(b.taskId).prio;

        if (prioA != prioB) {
            return prioA < prioB; // higher prio value = higher priority
        }
        return a.instanceNumber > b.instanceNumber;
    }
};


class SchedulingAlgorithm {
public:
    virtual ~SchedulingAlgorithm() = default;

    // Core method each algorithm must implement
    virtual Schedule GenerateSchedule(TaskSet &taskSet) = 0;
};