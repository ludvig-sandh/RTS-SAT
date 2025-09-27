#include "UniprocessorSchedulerBasedTest.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "UniprocessorSimulator.hpp"

void UniprocessorSchedulerBasedTest::CheckApplicability(const TaskSet& taskSet) const {
    // Always applicable
    (void)taskSet;
}

bool UniprocessorSchedulerBasedTest::RunTestImpl(const TaskSet& taskSet) const {
    std::unique_ptr<UniprocessorSchedulingAlgorithm> alg = GetScheduler();
    UniprocessorSimulator simulator(alg.get(), m_config); // Pass raw pointer since simulator doesn't take ownership.
    UniprocessorSchedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> RMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> DMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<DMScheduler>();
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> EDFSchedulabilityTest::GetScheduler() const {
    return std::make_unique<EDFScheduler>();
}