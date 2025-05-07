#include <cmath>
#include "EDFProcessorDemandTest.hpp"

bool EDFProcessorDemandTest::IsApplicable(const TaskSet &taskSet) const {
    return taskSet.IsSynchronous() && taskSet.HasConstrainedDeadlines();
}

bool EDFProcessorDemandTest::RunTestImpl(const TaskSet &taskSet) const {
    double utilization = taskSet.GetUtilization();
    uint32_t L_lcm = taskSet.GetHyperPeriod();
    
    // Compute L_max
    uint32_t L_max = L_lcm;
    if (utilization < 1) {
        // Compute L_brh
        uint32_t L_brh = 0;
        for (const PeriodicTask &task : taskSet.GetTasks()) {
            L_brh = std::max(L_brh, task.D);
        }

        double candidateL = 0.0;
        for (const PeriodicTask &task : taskSet.GetTasks()) {
            candidateL += (task.T - task.D) * static_cast<double>(task.C) / task.T;
        }
        candidateL /= (1 - utilization);

        L_brh = std::max(L_brh, static_cast<uint32_t>(std::ceil(candidateL)));

        L_max = std::min(L_brh, L_lcm);
    }

    // Test succeeds if and only if the cpu demand is <= controlPoint for each control point.
    std::unordered_set<uint32_t> controlPoints = taskSet.GetAbsoluteDeadlines();
    for (uint32_t controlPoint : controlPoints) {
        // We only need to check control points < L_max
        if (controlPoint > L_max) {
            continue;
        }

        uint32_t cpuDemand = ComputeProcessorDemand(taskSet, controlPoint);
        if (cpuDemand > controlPoint) {
            return false;
        }
    }

    return true;
}

uint32_t EDFProcessorDemandTest::ComputeProcessorDemand(const TaskSet &taskSet, uint32_t controlPoint) const {
    uint32_t cpuDemand = 0;
    for (const PeriodicTask &task : taskSet.GetTasks()) {
        cpuDemand += ((controlPoint - task.D) / task.T + 1) * task.C;
    }
    return cpuDemand;
}