#include <cmath>
#include "ResponseTimeAnalysisTest.hpp"

bool ResponseTimeAnalysisTest::IsApplicable(const TaskSet &taskSet) const {
    return taskSet.IsSynchronous() && taskSet.HasConstrainedDeadlines();
}

bool ResponseTimeAnalysisTest::RunTestImpl(const TaskSet &taskSet) const {
    // This test succeeds if and only if the response time for each task is <= its relative deadline
    for (const PeriodicTask &task : taskSet.GetTasks()) {
        uint32_t responseTime = ComputeResponseTime(taskSet, task);
        if (responseTime > task.D) {
            return false;
        }
    }

    return true;
}

uint32_t ResponseTimeAnalysisTest::ComputeResponseTime(const TaskSet &taskSet, const PeriodicTask &task) const {
    std::vector<PeriodicTask> higherPriorityTasks = taskSet.GetHigherPriorityTasks(task.prio);
    uint32_t estimatedResponseTime = task.C;
    uint32_t lastEstimatedResponseTime = UINT32_MAX; // Placeholder value

    // Run iterative process until convergence
    while (estimatedResponseTime != lastEstimatedResponseTime) {
        lastEstimatedResponseTime = estimatedResponseTime;

        // Update the estimation
        estimatedResponseTime = task.C;
        for (const PeriodicTask &higherPriorityTask : higherPriorityTasks) {
            estimatedResponseTime += ceil(static_cast<double>(lastEstimatedResponseTime) / higherPriorityTask.T) * higherPriorityTask.C;
        }
    }

    return estimatedResponseTime;
}