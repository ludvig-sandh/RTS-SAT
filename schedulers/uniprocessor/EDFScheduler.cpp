#include "EDFScheduler.hpp"

struct EarliestDeadlineComparator {
    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const {
        return b.deadline < a.deadline;
    }
};

UniprocessorSchedule EDFScheduler::GenerateSchedule(TaskSet& taskSet, bool allowPreemptions, uint32_t preemptionDelay) {
    UniprocessorSchedule schedule = GenerateScheduleImpl(taskSet, EarliestDeadlineComparator(), allowPreemptions, preemptionDelay);
    schedule.Validate();
    return schedule;
}