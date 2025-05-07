#include <algorithm>
#include <queue>
#include "RMScheduler.hpp"

void RMScheduler::AssignStaticPriorities(TaskSet& taskSet) {
    // Get local copy of the tasks in the task set
    std::vector<PeriodicTask> tasks = taskSet.GetCopyOfTasks();

    // Sort by period (descending), so lower T => higher priority
    std::sort(tasks.begin(), tasks.end(),
              [](const PeriodicTask& a, const PeriodicTask& b) {
                  return a.T > b.T;
              });
    
    // Assign priority (0 = lowest)
    for (std::size_t i = 0; i < tasks.size(); ++i) {
        taskSet.SetPriority(tasks[i].id, (uint32_t)i);
    }
}

Schedule RMScheduler::GenerateSchedule(TaskSet& taskSet) {
    // Assign priorities to each task in the set
    AssignStaticPriorities(taskSet);

    Schedule schedule(taskSet);

    // Get all task instances (sorted by ascending arrival time)
    std::deque<TaskJob> pendingJobs = taskSet.GetAllTaskJobs();
    uint32_t currentTime = 0;
    uint32_t hyperPeriod = taskSet.GetHyperPeriod();
    std::priority_queue<TaskJob, std::vector<TaskJob>, StaticPrioritiesComparator> readyQueue{
        StaticPrioritiesComparator(taskSet)
    };

    // Main simulation loop
    while (currentTime < hyperPeriod) {
        // Add any tasks that have been released up until now to the ready queue.
        while (pendingJobs.size() && pendingJobs.front().arrival <= currentTime) {
            readyQueue.push(pendingJobs.front());
            pendingJobs.pop_front();
        }

        // Get the next ready job with highest priority
        if (readyQueue.size()) {
            // Pop the job from the ready queue
            TaskJob jobToSchedule = readyQueue.top();
            readyQueue.pop();
            
            // Fill in the start and end times for this task
            uint32_t executionTime = taskSet.GetTask(jobToSchedule.taskId).C;
            jobToSchedule.start = currentTime;
            jobToSchedule.end = currentTime + executionTime;

            schedule.AddTaskJob(jobToSchedule);
            currentTime += executionTime;
        }else {
            // Advance time to next arrival, or if no more arrival exists, jump to the end of the hyperperiod
            if (pendingJobs.empty()) {
                currentTime = hyperPeriod;
            }else {
                currentTime = pendingJobs.front().arrival;
            }
        }
    }

    return schedule;
}