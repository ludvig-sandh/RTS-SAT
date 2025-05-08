#pragma once

#include "FeasibilityTest.hpp"

// Sufficient test
class LiuLaylandUtilizationBoundTest : public FeasibilityTest {
private:
    void CheckApplicability(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
    
    // Utilization bound is n(2^(1/n)-1) for n tasks
    double GetUtilizationBound(uint32_t numTasks) const;
};