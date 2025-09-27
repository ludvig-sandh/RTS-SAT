#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"

struct StaticPrioritiesComparator {
    const TaskSet& taskSet;

    StaticPrioritiesComparator(const TaskSet& ts) : taskSet(ts) {}

    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const;
};

// Base class for all static priority uniprocessor schedulers
class StaticPriorityScheduler : public UniprocessorSchedulingAlgorithm {
public:
    virtual ~StaticPriorityScheduler() = default;
    UniprocessorSchedule GenerateSchedule(const TaskSet& taskSet, bool allowPreemptions = true, uint32_t preemptionDelay = 0) override;
    
    virtual void AssignStaticPriorities(TaskSet& taskSet) const = 0;
};