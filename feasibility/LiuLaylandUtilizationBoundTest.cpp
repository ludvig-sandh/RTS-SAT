#include <cmath>
#include <stdexcept>
#include "LiuLaylandUtilizationBoundTest.hpp"

void LiuLaylandUtilizationBoundTest::CheckApplicability(const TaskSet &taskSet) const {
    if (!taskSet.IsSynchronous()) {
        throw InvalidFeasibilityTestException::CreateNonSynchronousException();
    }

    if (!taskSet.HasImplicitDeadlines()) {
        throw InvalidFeasibilityTestException::CreateNonImplicitDeadlinesException();
    }
}

bool LiuLaylandUtilizationBoundTest::RunTestImpl(const TaskSet &taskSet) const {
    uint32_t numTasks = taskSet.GetNumTasks();
    return taskSet.GetUtilization() <= GetUtilizationBound(numTasks);
}

double LiuLaylandUtilizationBoundTest::GetUtilizationBound(uint32_t numTasks) const {
    if (numTasks <= 0) {
        throw std::invalid_argument("Number of tasks must be positive.");
    }
    
    return numTasks * (pow(2.0, 1.0 / static_cast<double>(numTasks)) - 1.0);
}