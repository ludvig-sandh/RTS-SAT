#pragma once

#include "FeasibilityTest.hpp"

// Exact test
class ResponseTimeAnalysisTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;

    // Helper method to compute the response time for a given task, considering interference from higher priority tasks.
    uint32_t ComputeResponseTime(const TaskSet &taskSet, const PeriodicTask &task) const;
};