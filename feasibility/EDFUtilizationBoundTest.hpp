#pragma once
#include "FeasibilityTest.hpp"

// Exact test
class EDFUtilizationBoundTest : public FeasibilityTest {
private:
    void CheckApplicability(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
};