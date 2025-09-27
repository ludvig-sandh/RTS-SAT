#pragma once

#include <vector>
#include <stdexcept>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "UniprocessorSchedule.hpp"

// Base class for all uniprocessor scheduling algorithms, both static and dynamic
class UniprocessorSchedulingAlgorithm {
public:
    virtual ~UniprocessorSchedulingAlgorithm() = default;

    // Core method each algorithm must implement
    virtual UniprocessorSchedule GenerateSchedule(const TaskSet& taskSet, bool allowPreemptions, uint32_t preemptionDelay = 0) = 0;

protected:
    template <typename Comparator>
    UniprocessorSchedule GenerateScheduleImpl(const TaskSet& taskSet, Comparator priorityComp, bool allowPreemptions, uint32_t preemptionDelay) {
        UniprocessorSchedule schedule(taskSet);
    
        // Get all task instances (sorted by ascending arrival time)
        std::deque<TaskJob> pendingJobs = taskSet.GetAllTaskJobs();
    
        uint32_t currentTime = 0;
        uint32_t hyperPeriod = taskSet.GetHyperPeriod();
        std::priority_queue<TaskJob, std::vector<TaskJob>, Comparator> readyQueue{
            priorityComp
        };
        TaskJob *runningJob = nullptr;
    
        // Main simulation loop
        while (pendingJobs.size() || readyQueue.size() || runningJob) {
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
            }else if (runningJob && (readyQueue.empty() || !allowPreemptions || !priorityComp(*runningJob, readyQueue.top()))) {
                // Case 3: We have a running job, and no higher priority task has arrived, or preemptions are not allowed.
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
            }else if (runningJob && !readyQueue.empty() && priorityComp(*runningJob, readyQueue.top())) { // Case 4: We have a running job, and a higher priority task needs to preempt
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

                // Add delay due to preemption overhead
                currentTime += preemptionDelay;
            }else {
                throw std::runtime_error("Invalid case in GenerateScheduleImpl...");
            }
        }
    
        return schedule;
    }
};