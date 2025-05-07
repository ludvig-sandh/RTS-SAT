#include <algorithm>
#include "StaticPriorityScheduler.hpp"

bool StaticPrioritiesComparator::operator()(const TaskJob& a, const TaskJob& b) const {
    uint32_t prioA = taskSet.GetTask(a.taskId).prio;
    uint32_t prioB = taskSet.GetTask(b.taskId).prio;

    if (prioA != prioB) {
        return prioA < prioB; // higher prio value = higher priority
    }
    return a.instanceNumber > b.instanceNumber;
}

Schedule StaticPriorityScheduler::GenerateSchedule(TaskSet& taskSet, bool allowPreemptions) {
    // Assign priorities to each task in the set
    AssignStaticPriorities(taskSet);

    StaticPrioritiesComparator priorityComparator(taskSet);
    return GenerateScheduleImpl(taskSet, priorityComparator, allowPreemptions);
}