#include "EDFScheduler.hpp"

struct EarliestDeadlineComparator {
    // Should return true if b should come before a
    bool operator()(const TaskJob& a, const TaskJob& b) const {
        return b.deadline < a.deadline;
    }
};

Schedule EDFScheduler::GenerateSchedule(TaskSet& taskSet, bool allowPreemptions) {
    if (allowPreemptions) {
        return HelpGenerateScheduleWithPreemptions(taskSet);
    }

    return HelpGenerateScheduleWithoutPreemptions(taskSet);
}

Schedule EDFScheduler::HelpGenerateScheduleWithoutPreemptions(TaskSet &taskSet) {
    
    Schedule schedule(taskSet);

    // Get all task instances (sorted by ascending arrival time)
    std::deque<TaskJob> pendingJobs = taskSet.GetAllTaskJobs();
    uint32_t currentTime = 0;
    uint32_t hyperPeriod = taskSet.GetHyperPeriod();
    std::priority_queue<TaskJob, std::vector<TaskJob>, EarliestDeadlineComparator> readyQueue;

    // Main simulation loop
    while (currentTime < hyperPeriod) {
        // Add any tasks that have arrived up until now to the ready queue.
        while (pendingJobs.size() && pendingJobs.front().arrival <= currentTime) {
            readyQueue.push(pendingJobs.front());
            pendingJobs.pop_front();
        }

        // Get the next job with earliest deadline
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

Schedule EDFScheduler::HelpGenerateScheduleWithPreemptions(TaskSet &taskSet) {
    return GenerateScheduleImpl(taskSet, EarliestDeadlineComparator(), true);
}
