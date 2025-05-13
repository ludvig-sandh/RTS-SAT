#pragma once

#include <string>
#include <stdint.h>

struct PeriodicTask {
    uint32_t C; // WCET
    uint32_t D; // Deadline
    uint32_t T; // Period
    uint32_t O; // Offset
    std::string id; // Unique identifier for this task
    uint32_t prio; // Higher value <=> higher priority

    PeriodicTask(uint32_t C, uint32_t D, uint32_t T, const std::string &id);
    PeriodicTask(uint32_t C, uint32_t D, uint32_t T, uint32_t O, const std::string &id);

private:
    void CheckNonZeroPeriod() const;
};

struct TaskJob {
    uint32_t arrival; // Absolute arrival time of this task instance
    uint32_t deadline; // Absolute deadline time of this task instance
    uint32_t instanceNumber; // Task instance number
    std::string taskId; // Id of task it is an instance of
    uint32_t remainingTime; // If task is preempted, this keeps track of the remaining time to execute
    uint32_t start; // The time the job is scheduled - is set by the scheduler at a later stage
    uint32_t end; // The time the job completes (or is preempted) - is set by the scheduler at a later stage

    TaskJob(uint32_t arrival, uint32_t deadline, uint32_t instanceNumber, const std::string &taskId, uint32_t remainingTime) :
        arrival(arrival), deadline(deadline), instanceNumber(instanceNumber), taskId(taskId), remainingTime(remainingTime), start(UINT32_MAX), end(UINT32_MAX) {};

    TaskJob(uint32_t arrival, uint32_t deadline, uint32_t instanceNumber, const std::string &taskId, uint32_t remainingTime, uint32_t start, uint32_t end) :
        arrival(arrival), deadline(deadline), instanceNumber(instanceNumber), taskId(taskId), remainingTime(remainingTime), start(start), end(end) {};

    void Print() const;
};