#include "EDFUtilizationBoundTest.hpp"

void EDFUtilizationBoundTest::CheckApplicability(const TaskSet& taskSet) const {
    if (!taskSet.IsSynchronous()) {
        throw InvalidFeasibilityTestException::CreateNonSynchronousException();
    }

    if (!taskSet.HasImplicitDeadlines()) {
        throw InvalidFeasibilityTestException::CreateNonImplicitDeadlinesException();
    }
}

bool EDFUtilizationBoundTest::RunTestImpl(const TaskSet& taskSet) const {
    return taskSet.GetUtilization() <= 1.0;
}