#pragma once

#include <memory>
#include "FeasibilityTest.hpp"
#include "UniprocessorSchedulingAlgorithm.hpp"

// Exact tests

class UniprocessorSchedulerBasedTest : public FeasibilityTest {
private:
    bool m_allowPreemptions;
    uint32_t m_preemptionDelay;

    void CheckApplicability(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;

    virtual std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const = 0;

public:
    UniprocessorSchedulerBasedTest(bool allowPreemptions = true, uint32_t preemptionDelay = 0)
        : m_allowPreemptions(allowPreemptions), m_preemptionDelay(preemptionDelay) {};
};

class RMSchedulabilityTest : public UniprocessorSchedulerBasedTest {
private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;

public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;
};

class DMSchedulabilityTest : public UniprocessorSchedulerBasedTest {
private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;

public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;
};

class EDFSchedulabilityTest : public UniprocessorSchedulerBasedTest {
private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;

public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;
};