#include <algorithm>
#include "RMFFScheduler.hpp"
#include "RMScheduler.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"

MultiprocessorSchedule RMFFScheduler::GenerateScheduleImpl(TaskSet &taskSet, uint32_t numCpus) {
    // Assign RM priorities to the taskset
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);

    // Get a list of the tasks and sort them by highest priority
    std::vector<PeriodicTask> tasks = taskSet.GetTasks();
    std::sort(tasks.begin(), tasks.end(), [](const PeriodicTask &a, const PeriodicTask &b)
    { 
        return a.prio > b.prio; 
    });

    // Represents the task allocation for processors
    std::vector<std::vector<PeriodicTask>> cpuTaskSets(numCpus);

    LiuLaylandUtilizationBoundTest feasibilityTest;
    for (const PeriodicTask &task : tasks) {
        // Try fit this task in any of the processors using the utilization bound feasibility test
        for (uint32_t cpuIdx = 0; cpuIdx < numCpus; cpuIdx++) {
            cpuTaskSets[cpuIdx].push_back(task);
            if (feasibilityTest.RunTest(TaskSet(cpuTaskSets[cpuIdx]))) {
                // This task has been successfully assigned to this processor
                break;
            }

            // Couldn't be assigned, so remove from this processor
            cpuTaskSets[cpuIdx].pop_back();
        }
    }

    // Now schedule all tasks with RM in each processor
    MultiprocessorSchedule multiSchedule(taskSet, numCpus);
    for (uint32_t cpuIdx = 0; cpuIdx < numCpus; cpuIdx++) {
        TaskSet taskSet(cpuTaskSets[cpuIdx]);
        UniprocessorSchedule uniSchedule = rm.GenerateSchedule(taskSet); // We know RM returns a unique ptr to uniprocessor schedule
        multiSchedule.SetScheduleOfCore(uniSchedule, cpuIdx);
    }

    return multiSchedule;
}