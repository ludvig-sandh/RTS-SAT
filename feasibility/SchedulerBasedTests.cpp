#include "SchedulerBasedTests.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "Simulator.hpp"

bool RMSchedulabilityTest::IsApplicable(const TaskSet &taskSet) const {
    (void)taskSet;
    return true;
}

bool RMSchedulabilityTest::RunTestImpl(const TaskSet &taskSet) const {
    RMScheduler scheduler;
    Simulator simulator(&scheduler);
    Schedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

bool DMSchedulabilityTest::IsApplicable(const TaskSet &taskSet) const {
    (void)taskSet;
    return true;
}

bool DMSchedulabilityTest::RunTestImpl(const TaskSet &taskSet) const {
    DMScheduler scheduler;
    Simulator simulator(&scheduler);
    Schedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}

bool EDFSchedulabilityTest::IsApplicable(const TaskSet &taskSet) const {
    (void)taskSet;
    return true;
}

bool EDFSchedulabilityTest::RunTestImpl(const TaskSet &taskSet) const {
    EDFScheduler scheduler;
    Simulator simulator(&scheduler);
    Schedule schedule = simulator.run(taskSet);
    return schedule.AreDeadlinesMet(false);
}