// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#include <stdexcept>
#include "BaseFeasibilityTest.hpp"

bool BaseFeasibilityTest::RunTest(const TaskSet& taskSet) {
    CheckApplicability(taskSet);
    return RunTestImpl(taskSet);
}

InvalidFeasibilityTestException InvalidFeasibilityTestException::CreateNonSynchronousException() {
    return InvalidFeasibilityTestException("Feasibility test is not applicable to the given task set, because it is not synchronous.");
}

InvalidFeasibilityTestException InvalidFeasibilityTestException::CreateNonImplicitDeadlinesException() {
    return InvalidFeasibilityTestException("Feasibility test is not applicable to the given task set, because the deadlines are not implicit.");
}

InvalidFeasibilityTestException InvalidFeasibilityTestException::CreateNonConstrainedDeadlinesException() {
    return InvalidFeasibilityTestException("Feasibility test is not applicable to the given task set, because the deadlines are not constrained.");
}