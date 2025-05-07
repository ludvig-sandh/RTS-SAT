#pragma once
#include "FeasibilityTest.hpp"

// Exact test
class EDFProcessorDemandTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
    uint32_t ComputeProcessorDemand(const TaskSet &taskSet, uint32_t controlPoint) const;
};