#pragma once

#include "FeasibilityTest.hpp"

class LiuLaylandUtilizationBoundTest : public FeasibilityTest {
public:
    bool RunTest(const TaskSet &taskSet) const override;
private:
    // Utilization bound is n(2^(1/n)-1) for n tasks
    double GetUtilizationBound(uint32_t numTasks) const;
};