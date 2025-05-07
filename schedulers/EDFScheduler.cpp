#include "EDFScheduler.hpp"

struct EarliestDeadlineComparator {
    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const {
        return b.deadline < a.deadline;
    }
};

Schedule EDFScheduler::GenerateSchedule(TaskSet& taskSet, bool allowPreemptions) {
    return GenerateScheduleImpl(taskSet, EarliestDeadlineComparator(), allowPreemptions);
}