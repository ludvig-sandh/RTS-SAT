#include <algorithm>
#include "StaticPriorityScheduler.hpp"

bool StaticPrioritiesComparator::operator()(const TaskJob& a, const TaskJob& b) const {
    uint32_t prioA = taskSet.GetTask(a.taskId).prio;
    uint32_t prioB = taskSet.GetTask(b.taskId).prio;

    if (prioA != prioB) {
        return prioA < prioB; // higher prio value = higher priority
    }
    return a.instanceNumber > b.instanceNumber;
}

Schedule StaticPriorityScheduler::GenerateSchedule(TaskSet& taskSet, bool allowPreemptions) {
    // Assign priorities to each task in the set
    AssignStaticPriorities(taskSet);

    if (allowPreemptions) {
        return HelpGenerateScheduleWithPreemptions(taskSet);
    }

    return HelpGenerateScheduleWithoutPreemptions(taskSet);
}

Schedule StaticPriorityScheduler::HelpGenerateScheduleWithoutPreemptions(TaskSet &taskSet) {
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
        // Add any tasks that have arrived up until now to the ready queue.
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

Schedule StaticPriorityScheduler::HelpGenerateScheduleWithPreemptions(TaskSet &taskSet) {
    StaticPrioritiesComparator priorityComparator(taskSet);
    return GenerateScheduleImpl(taskSet, priorityComparator, true);
}
