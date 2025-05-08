#pragma once

#include <memory>
#include "FeasibilityTest.hpp"
#include "SchedulingAlgorithm.hpp"

// Exact tests

class SchedulerBasedTest : public FeasibilityTest {
private:
    bool m_allowPreemptions = true;
    uint32_t m_preemptionDelay = 0;

    bool IsApplicable(const TaskSet &taskSet) const override;
    bool RunTestImpl(const TaskSet &taskSet) const override;

    virtual std::unique_ptr<SchedulingAlgorithm> GetScheduler() const = 0;

public:
    SchedulerBasedTest() = default;
    SchedulerBasedTest(bool allowPreemptions)
        : m_allowPreemptions(allowPreemptions) {};
    SchedulerBasedTest(bool allowPreemptions, uint32_t preemptionDelay)
        : m_allowPreemptions(allowPreemptions), m_preemptionDelay(preemptionDelay) {};
};

class RMSchedulabilityTest : public SchedulerBasedTest {
private:
    std::unique_ptr<SchedulingAlgorithm> GetScheduler() const override;

public:
    using SchedulerBasedTest::SchedulerBasedTest;
};

class DMSchedulabilityTest : public SchedulerBasedTest {
private:
    std::unique_ptr<SchedulingAlgorithm> GetScheduler() const override;

public:
    using SchedulerBasedTest::SchedulerBasedTest;
};

class EDFSchedulabilityTest : public SchedulerBasedTest {
private:
    std::unique_ptr<SchedulingAlgorithm> GetScheduler() const override;

public:
    using SchedulerBasedTest::SchedulerBasedTest;
};