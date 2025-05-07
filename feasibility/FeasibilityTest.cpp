#include "FeasibilityTest.hpp"

double FeasibilityTest::GetUtilization(const TaskSet &taskSet) const {
    double utilization = 0.0;
    for (const PeriodicTask &task : taskSet.GetTasks()) {
        utilization += static_cast<double>(task.C) / static_cast<double>(task.T);
    }
    return utilization;
}