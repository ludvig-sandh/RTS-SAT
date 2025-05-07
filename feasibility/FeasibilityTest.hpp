#pragma once

#include "TaskSet.hpp"

class FeasibilityTest {
public:
    virtual ~FeasibilityTest() = default;

    // Template method: final to prevent override
    virtual bool RunTest(const TaskSet& taskSet) final;

    double GetUtilization(const TaskSet &taskSet) const;
    
protected:
    virtual bool IsApplicable(const TaskSet &taskSet) const = 0;
    virtual bool RunTestImpl(const TaskSet &taskSet) const = 0;

};