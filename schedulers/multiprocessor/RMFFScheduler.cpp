#include <algorithm>
#include "RMFFScheduler.hpp"
#include "RMScheduler.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"

MultiprocessorSchedule RMFFScheduler::GenerateScheduleImpl(const TaskSet& taskSet, uint32_t numCpus) {
    TaskSet mutableTaskSet(taskSet);

    // Assign RM priorities to the taskset
    RMScheduler rm;
    rm.AssignStaticPriorities(mutableTaskSet);

    // Get a vector of the tasks and sort them by highest priority
    std::vector<PeriodicTask> tasks;
    for (const PeriodicTask& t : mutableTaskSet) {
        tasks.push_back(t);
    }
    
    std::sort(tasks.begin(), tasks.end(), [](const PeriodicTask& a, const PeriodicTask& b)
    { 
        return a.prio > b.prio; 
    });

    // Represents the task allocation for processors
    std::vector<std::vector<PeriodicTask>> cpuTaskSets(numCpus);

    LiuLaylandUtilizationBoundTest BaseFeasibilityTest;
    for (const PeriodicTask& task : tasks) {
        // Try fit this task in any of the processors using the utilization bound feasibility test
        for (uint32_t cpuIdx = 0; cpuIdx < numCpus; cpuIdx++) {
            cpuTaskSets[cpuIdx].push_back(task);
            if (BaseFeasibilityTest.RunTest(TaskSet(cpuTaskSets[cpuIdx]))) {
                // This task has been successfully assigned to this processor
                break;
            }

            // Couldn't be assigned, so remove from this processor
            cpuTaskSets[cpuIdx].pop_back();
        }
    }

    // Now schedule all tasks with RM in each processor
    MultiprocessorSchedule multiSchedule(mutableTaskSet, numCpus);
    for (uint32_t cpuIdx = 0; cpuIdx < numCpus; cpuIdx++) {
        TaskSet uniprocessorTaskSet(cpuTaskSets[cpuIdx]);
        UniprocessorSchedule uniSchedule = rm.GenerateSchedule(uniprocessorTaskSet);
        multiSchedule.SetScheduleOfCore(uniSchedule, cpuIdx);
    }

    return multiSchedule;
}