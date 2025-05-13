#include <cmath>
#include "EDFProcessorDemandTest.hpp"

void EDFProcessorDemandTest::CheckApplicability(const TaskSet &taskSet) const {
    if (!taskSet.IsSynchronous()) {
        throw InvalidFeasibilityTestException::CreateNonSynchronousException();
    }

    if (!taskSet.HasConstrainedDeadlines()) {
        throw InvalidFeasibilityTestException::CreateNonConstrainedDeadlinesException();
    }
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
    for (int32_t controlPoint : controlPoints) { // Convert to signed intentionally
        // We only need to check control points < L_max
        if ((uint32_t)controlPoint > L_max) {
            continue;
        }

        int32_t cpuDemand = ComputeProcessorDemand(taskSet, controlPoint);
        if (cpuDemand > controlPoint) {
            return false;
        }
    }

    return true;
}

int32_t EDFProcessorDemandTest::ComputeProcessorDemand(const TaskSet &taskSet, int32_t controlPoint) const {
    int32_t cpuDemand = 0;
    for (const PeriodicTask &task : taskSet.GetTasks()) {
        int32_t valueToFloorWithT = controlPoint - (int32_t)task.D;
        if (valueToFloorWithT < 0) {
            // Make sure flooring works for negative numbers by moving down to closest negative multiple of T
            valueToFloorWithT = valueToFloorWithT + (-valueToFloorWithT) % (int32_t)task.T - task.T;
        }
        cpuDemand += (valueToFloorWithT / (int32_t)task.T + 1) * (int32_t)task.C;
    }
    return cpuDemand;
}