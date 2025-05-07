#include <algorithm>
#include "DMScheduler.hpp"

void DMScheduler::AssignStaticPriorities(TaskSet& taskSet) {
    // Get local copy of the tasks in the task set
    std::vector<PeriodicTask> tasks = taskSet.GetCopyOfTasks();

    // Sort by period (descending), so lower T => higher priority
    std::sort(tasks.begin(), tasks.end(),
              [](const PeriodicTask& a, const PeriodicTask& b) {
                  return a.D > b.D;
              });
    
    // Assign priority (0 = lowest)
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        taskSet.SetPriority(tasks[i].id, (uint32_t)i);
    }
}