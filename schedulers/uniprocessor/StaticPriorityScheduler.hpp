#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"

struct StaticPrioritiesComparator {
    const TaskSet& taskSet;

    StaticPrioritiesComparator(const TaskSet& ts) : taskSet(ts) {}

    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const;
};

class StaticPriorityScheduler : public UniprocessorSchedulingAlgorithm {
public:
    UniprocessorSchedule GenerateSchedule(TaskSet &taskSet, bool allowPreemptions = true, uint32_t preemptionDelay = 0) override;
    
    virtual void AssignStaticPriorities(TaskSet& taskSet) const = 0;
};