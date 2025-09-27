#include <algorithm>
#include "DMScheduler.hpp"

void DMScheduler::AssignStaticPriorities(TaskSet& taskSet) const {
    // Get local copy of the tasks in the task set
    TaskSet copiedTaskSet(taskSet);

    // Sort by period (descending), so lower T => higher priority
    std::sort(copiedTaskSet.begin(), copiedTaskSet.end(),
              [](const PeriodicTask& a, const PeriodicTask& b) {
                  return a.D > b.D;
              });
    
    // Assign priority (0 = lowest)
    uint32_t i = 0;
    for (auto it = copiedTaskSet.begin(); it != copiedTaskSet.end(); it++, i++) {
        taskSet.SetPriority(it->id, i);
    }
}