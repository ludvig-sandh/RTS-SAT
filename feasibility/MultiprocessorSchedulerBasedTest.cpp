// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#include "MultiprocessorSchedulerBasedTest.hpp"
#include "RMFFScheduler.hpp"
#include "MultiprocessorSimulator.hpp"

void MultiprocessorSchedulerBasedTest::CheckApplicability(const TaskSet& taskSet) const {
    (void)taskSet;
}

bool MultiprocessorSchedulerBasedTest::RunTestImpl(const TaskSet& taskSet) const {
    std::unique_ptr<MultiprocessorSchedulingAlgorithm> alg = GetScheduler();
    MultiprocessorSimulator::Config config(m_numCpus);
    MultiprocessorSimulator simulator(alg.get(), config); // Pass raw pointer since simulator doesn't take ownership.
    MultiprocessorSchedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

std::unique_ptr<MultiprocessorSchedulingAlgorithm> RMFFSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMFFScheduler>();
}