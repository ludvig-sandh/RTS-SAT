#pragma once

#include "BaseFeasibilityTest.hpp"

// Sufficient test
class RMFFUtilizationBoundTest : public BaseFeasibilityTest {
private:
    uint32_t m_numCpus;

    void CheckApplicability(const TaskSet& taskSet) const override;
    bool RunTestImpl(const TaskSet& taskSet) const override;
    
    // Utilization bound is m(sqrt(2)-1) for m processors
    double GetUtilizationBound() const;

public:
    RMFFUtilizationBoundTest(uint32_t numCpus);
};