// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#include <cmath>
#include "ResponseTimeAnalysisTest.hpp"

void ResponseTimeAnalysisTest::CheckApplicability(const TaskSet& taskSet) const {
    for (const PeriodicTask& task : taskSet) {
        if (task.prio == UINT32_MAX) {
            // Unnassigned priority. RTA assumes static priorities have been assigned.
            throw InvalidFeasibilityTestException("Feasibility test is not applicable to the given task set, because static priorities have not been set.");
        }
    }
    
    if (!taskSet.IsSynchronous()) {
        throw InvalidFeasibilityTestException::CreateNonSynchronousException();
    }

    if (!taskSet.HasConstrainedDeadlines()) {
        throw InvalidFeasibilityTestException::CreateNonConstrainedDeadlinesException();
    }
}

bool ResponseTimeAnalysisTest::RunTestImpl(const TaskSet& taskSet) const {
    // This test succeeds if and only if the response time for each task is <= its relative deadline
    for (const PeriodicTask& task : taskSet) {
        uint32_t responseTime = ComputeResponseTime(taskSet, task);
        if (responseTime > task.D) {
            return false;
        }
    }

    return true;
}

uint32_t ResponseTimeAnalysisTest::ComputeResponseTime(const TaskSet& taskSet, const PeriodicTask& task) const {
    TaskSet higherPriorityTasks = taskSet.GetHigherPriorityTasks(task.prio);
    uint32_t estimatedResponseTime = task.C;
    uint32_t lastEstimatedResponseTime = UINT32_MAX; // Placeholder value

    // Run iterative process until convergence
    while (estimatedResponseTime != lastEstimatedResponseTime && estimatedResponseTime <= task.D) {
        lastEstimatedResponseTime = estimatedResponseTime;

        // Update the estimation
        estimatedResponseTime = task.C;
        for (const PeriodicTask& higherPriorityTask : higherPriorityTasks) {
            estimatedResponseTime += ceil(static_cast<double>(lastEstimatedResponseTime) / higherPriorityTask.T) * higherPriorityTask.C;
        }
    }

    return estimatedResponseTime;
}