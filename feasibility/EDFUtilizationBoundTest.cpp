#include "EDFUtilizationBoundTest.hpp"

bool EDFUtilizationBoundTest::IsApplicable(const TaskSet &taskSet) const {
    return taskSet.IsSynchronous() && taskSet.HasImplicitDeadlines();
}

bool EDFUtilizationBoundTest::RunTestImpl(const TaskSet &taskSet) const {
    return taskSet.GetUtilization() <= 1.0;
}