#pragma once
#include "FeasibilityTest.hpp"

// Exact test
class EDFUtilizationBoundTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
};