#include "SchedulerBasedTests.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "Simulator.hpp"

void SchedulerBasedTest::CheckApplicability(const TaskSet &taskSet) const {
    (void)taskSet;
}

bool SchedulerBasedTest::RunTestImpl(const TaskSet &taskSet) const {
    std::unique_ptr<SchedulingAlgorithm> alg = GetScheduler();
    Simulator simulator(alg.get()); // Pass raw pointer since simulator doesn't take ownership.
    simulator.SetPreemptionsAllowed(m_allowPreemptions);
    simulator.SetPreemptionDelay(m_preemptionDelay);

    UniprocessorSchedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

std::unique_ptr<SchedulingAlgorithm> RMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}

std::unique_ptr<SchedulingAlgorithm> DMSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}

std::unique_ptr<SchedulingAlgorithm> EDFSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMScheduler>();
}