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
    Schedule schedule(taskSet);

    // Get all task instances (sorted by ascending arrival time)
    std::deque<TaskJob> pendingJobs = taskSet.GetAllTaskJobs();

    uint32_t currentTime = 0;
    uint32_t hyperPeriod = taskSet.GetHyperPeriod();
    StaticPrioritiesComparator priorityComparator(taskSet);
    std::priority_queue<TaskJob, std::vector<TaskJob>, StaticPrioritiesComparator> readyQueue{
        priorityComparator
    };
    TaskJob *runningJob = nullptr;

    // Main simulation loop
    while (currentTime < hyperPeriod) {
        // Add any tasks that have arrived up until now to the ready queue.
        while (pendingJobs.size() && pendingJobs.front().arrival <= currentTime) {
            readyQueue.push(pendingJobs.front());
            pendingJobs.pop_front();
        }

        // Split into different cases:
        if (runningJob == nullptr && readyQueue.empty()) { // Case 1: We don't have a running job, and no task has arrived
            // Jump to next arrival, or to end of hyper period if no more arrivals
            currentTime = pendingJobs.empty() ? hyperPeriod : pendingJobs.front().arrival;
        }else if (runningJob == nullptr && !readyQueue.empty()) { // Case 2: We don't have a running job, but some task has arrived
            // Pop the job from the ready queue
            runningJob = new TaskJob(readyQueue.top());
            readyQueue.pop();
            runningJob->start = currentTime;
            // Now we have a running job, so let next iteration of the loop decide which case we are in
        }else if (runningJob && (readyQueue.empty() || !priorityComparator(*runningJob, readyQueue.top()))) { // Case 3: We have a running job, and no higher priority task has arrived
            uint32_t currentJobFinishTime = runningJob->start + runningJob->remainingTime;
            uint32_t nextArrivalTime = pendingJobs.empty() ? UINT32_MAX : pendingJobs.front().arrival;
            if (pendingJobs.empty() || currentJobFinishTime <= nextArrivalTime) {
                // The running job finishes without interruption
                runningJob->end = currentJobFinishTime;
                runningJob->remainingTime = 0;

                // Add the finished job to the schedule and free up memory
                schedule.AddTaskJob(*runningJob);
                delete runningJob;
                runningJob = nullptr;

                // Advance time
                currentTime = currentJobFinishTime;
            }else {
                // A task will arrive before this job finishes, so advance time to let the task arrive, and decide then what to do in next iteration
                currentTime = nextArrivalTime;
            }
        }else if (runningJob && !readyQueue.empty() && priorityComparator(*runningJob, readyQueue.top())) { // Case 4: We have a running job, and a higher priority task needs to preempt
            // Compute how much time this task instance has left to execute
            runningJob->end = currentTime;
            uint32_t jobDuration = runningJob->end - runningJob->start;
            runningJob->remainingTime = std::max(runningJob->remainingTime - jobDuration, (uint32_t)0);

            // Add this job to the schedule
            schedule.AddTaskJob(*runningJob);

            // If there is anything left to execute of this task instance, add it back to the queue
            if (runningJob->remainingTime > 0) {
                runningJob->start = UINT32_MAX;
                runningJob->end = UINT32_MAX;
                readyQueue.push(*runningJob);
            }

            delete runningJob;
            runningJob = nullptr;
        }
    }

    return schedule;
}
