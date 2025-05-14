#include "UniprocessorSchedulerBasedTests.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "Simulator.hpp"

void UniprocessorSchedulerBasedTest::CheckApplicability(const TaskSet &taskSet) const {
    (void)taskSet;
}

bool UniprocessorSchedulerBasedTest::RunTestImpl(const TaskSet &taskSet) const {
    std::unique_ptr<UniprocessorSchedulingAlgorithm> alg = GetScheduler();
    Simulator simulator(alg.get()); // Pass raw pointer since simulator doesn't take ownership.
    simulator.SetPreemptionsAllowed(m_allowPreemptions);
    simulator.SetPreemptionDelay(m_preemptionDelay);

    UniprocessorSchedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> RMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> DMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}

std::unique_ptr<UniprocessorSchedulingAlgorithm> EDFSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}