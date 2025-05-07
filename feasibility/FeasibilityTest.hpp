#pragma once

#include "TaskSet.hpp"

class FeasibilityTest {
public:
    virtual ~FeasibilityTest() = default;

    virtual bool RunTest(const TaskSet &taskSet) const = 0;

    double GetUtilization(const TaskSet &taskSet) const;
};