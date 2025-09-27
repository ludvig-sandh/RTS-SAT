// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <memory>
#include "BaseFeasibilityTest.hpp"
#include "MultiprocessorSchedulingAlgorithm.hpp"
#include "MultiprocessorSimulator.hpp"

// Base class for exact feasibility tests (multiprocessor)
class MultiprocessorSchedulerBasedTest : public BaseFeasibilityTest {
public:
    MultiprocessorSchedulerBasedTest(uint32_t numCpus) : m_numCpus(numCpus) {};
    virtual ~MultiprocessorSchedulerBasedTest() = default;

private:
    uint32_t m_numCpus;

    void CheckApplicability(const TaskSet& taskSet) const override;
    bool RunTestImpl(const TaskSet& taskSet) const override;

    virtual std::unique_ptr<MultiprocessorSchedulingAlgorithm> GetScheduler() const = 0;
};

class RMFFSchedulabilityTest : public MultiprocessorSchedulerBasedTest {
public:
    using MultiprocessorSchedulerBasedTest::MultiprocessorSchedulerBasedTest;

private:
    std::unique_ptr<MultiprocessorSchedulingAlgorithm> GetScheduler() const override;
};