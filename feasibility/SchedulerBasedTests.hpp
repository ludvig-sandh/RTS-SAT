#pragma once

#include "FeasibilityTest.hpp"

// Exact tests

class RMSchedulabilityTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
};

class DMSchedulabilityTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
};

class EDFSchedulabilityTest : public FeasibilityTest {
private:
    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;
};