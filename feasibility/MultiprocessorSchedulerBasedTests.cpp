#include "MultiprocessorSchedulerBasedTests.hpp"
#include "RMFFScheduler.hpp"

void MultiprocessorSchedulerBasedTest::CheckApplicability(const TaskSet &taskSet) const {
    (void)taskSet;
}

bool MultiprocessorSchedulerBasedTest::RunTestImpl(const TaskSet &taskSet) const {
    std::unique_ptr<MultiprocessorSchedulingAlgorithm> alg = GetScheduler();
    TaskSet mutableTaskSet = taskSet;
    MultiprocessorSchedule schedule = alg->GenerateSchedule(mutableTaskSet, m_numCpus);
    return schedule.AreDeadlinesMet(false);
}

std::unique_ptr<MultiprocessorSchedulingAlgorithm> RMFFSchedulabilityTest::GetScheduler() const {
    return std::make_unique<RMFFScheduler>();
}