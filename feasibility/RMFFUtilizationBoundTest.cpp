#include <cmath>
#include <stdexcept>
#include <algorithm>
#include "RMFFUtilizationBoundTest.hpp"
#include "MultiprocessorSchedule.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"
#include "RMScheduler.hpp"

RMFFUtilizationBoundTest::RMFFUtilizationBoundTest(uint32_t numCpus) : m_numCpus(numCpus) {
    if (numCpus == 0) {
        throw std::invalid_argument("Cannot perform RMFF utilization bound test with zero processors.");
    }
}

double RMFFUtilizationBoundTest::GetUtilizationBound() const {
    return m_numCpus * (sqrt(2) - 1.0);
}

void RMFFUtilizationBoundTest::CheckApplicability(const TaskSet &taskSet) const {
    if (!taskSet.IsSynchronous()) {
        throw InvalidFeasibilityTestException::CreateNonSynchronousException();
    }

    if (!taskSet.HasImplicitDeadlines()) {
        throw InvalidFeasibilityTestException::CreateNonImplicitDeadlinesException();
    }
}

bool RMFFUtilizationBoundTest::RunTestImpl(const TaskSet &taskSet) const {
    return taskSet.GetUtilization() <= GetUtilizationBound();
}