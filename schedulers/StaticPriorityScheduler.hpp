#pragma once

#include "SchedulingAlgorithm.hpp"

struct StaticPrioritiesComparator {
    const TaskSet& taskSet;

    StaticPrioritiesComparator(const TaskSet& ts) : taskSet(ts) {}

    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const {
        uint32_t prioA = taskSet.GetTask(a.taskId).prio;
        uint32_t prioB = taskSet.GetTask(b.taskId).prio;

        if (prioA != prioB) {
            return prioA < prioB; // higher prio value = higher priority
        }
        return a.instanceNumber > b.instanceNumber;
    }
};

class StaticPriorityScheduler : public SchedulingAlgorithm {
public:
    Schedule GenerateSchedule(TaskSet &taskSet, bool allowPreemptions) override;
private:
    Schedule HelpGenerateScheduleWithoutPreemptions(TaskSet &taskSet);
    Schedule HelpGenerateScheduleWithPreemptions(TaskSet &taskSet);
    
    virtual void AssignStaticPriorities(TaskSet& taskSet) = 0;
};