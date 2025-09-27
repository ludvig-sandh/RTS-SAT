#pragma once
#include "BaseFeasibilityTest.hpp"

// Exact test
class EDFProcessorDemandTest : public BaseFeasibilityTest {
private:
    void CheckApplicability(const TaskSet& taskSet) const override;
    bool RunTestImpl(const TaskSet& taskSet) const override;
    int32_t ComputeProcessorDemand(const TaskSet& taskSet, int32_t controlPoint) const;
};