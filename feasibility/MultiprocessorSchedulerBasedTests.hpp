#pragma once

#include <memory>
#include "FeasibilityTest.hpp"
#include "MultiprocessorSchedulingAlgorithm.hpp"

// Exact tests

class MultiprocessorSchedulerBasedTest : public FeasibilityTest {
private:
    uint32_t m_numCpus;

    void CheckApplicability(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;

    virtual std::unique_ptr<MultiprocessorSchedulingAlgorithm> GetScheduler() const = 0;

public:
    MultiprocessorSchedulerBasedTest(uint32_t numCpus) : m_numCpus(numCpus) {};
};

class RMFFSchedulabilityTest : public MultiprocessorSchedulerBasedTest {
private:
    std::unique_ptr<MultiprocessorSchedulingAlgorithm> GetScheduler() const override;

public:
    using MultiprocessorSchedulerBasedTest::MultiprocessorSchedulerBasedTest;
};