#pragma once

#include "TaskSet.hpp"

class FeasibilityTest {
public:
    virtual ~FeasibilityTest() = default;

    // Template method: final to prevent override
    virtual bool RunTest(const TaskSet& taskSet) final;

protected:
    // Checks if this feasibility test is applicable to the given task set
    virtual bool IsApplicable(const TaskSet &taskSet) const = 0;

    // Runs the feasibility test on a given task set, given that it is applicable
    virtual bool RunTestImpl(const TaskSet &taskSet) const = 0;
};