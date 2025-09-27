// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <memory>
#include "BaseFeasibilityTest.hpp"
#include "UniprocessorSchedulingAlgorithm.hpp"
#include "UniprocessorSimulator.hpp"

// Base class for exact feasibility tests (uniprocessor)
class UniprocessorSchedulerBasedTest : public BaseFeasibilityTest {
public:
    virtual ~UniprocessorSchedulerBasedTest() = default;

protected:
    UniprocessorSchedulerBasedTest() : m_config(UniprocessorSimulator::CreateDefaultConfig()) {}
    explicit UniprocessorSchedulerBasedTest(const UniprocessorSimulator::Config& config)
        : m_config(config) {}

private:
    UniprocessorSimulator::Config m_config;
    void CheckApplicability(const TaskSet& taskSet) const override;
    bool RunTestImpl(const TaskSet& taskSet) const override;

    virtual std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const = 0;
};

class RMSchedulabilityTest : public UniprocessorSchedulerBasedTest {
public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;

private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;
};

class DMSchedulabilityTest : public UniprocessorSchedulerBasedTest {
public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;

private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;
};

class EDFSchedulabilityTest : public UniprocessorSchedulerBasedTest {
public:
    using UniprocessorSchedulerBasedTest::UniprocessorSchedulerBasedTest;

private:
    std::unique_ptr<UniprocessorSchedulingAlgorithm> GetScheduler() const override;
};