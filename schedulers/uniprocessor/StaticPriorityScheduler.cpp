// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

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

UniprocessorSchedule StaticPriorityScheduler::GenerateSchedule(const TaskSet& taskSet, bool allowPreemptions, uint32_t preemptionDelay) {
    // Assign priorities to each task in the set
    TaskSet mutableTaskSet(taskSet);
    AssignStaticPriorities(mutableTaskSet);

    StaticPrioritiesComparator priorityComparator(mutableTaskSet);
    UniprocessorSchedule schedule = GenerateScheduleImpl(mutableTaskSet, priorityComparator, allowPreemptions, preemptionDelay);
    schedule.Validate();
    return schedule;
}