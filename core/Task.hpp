#pragma once

#include <stdint.h>

struct PeriodicTask {
    uint32_t C; // WCET
    uint32_t D; // Deadline
    uint32_t T; // Period
    uint32_t id; // Unique identifier for this task
    uint32_t prio; // Higher value <=> higher priority

    PeriodicTask(uint32_t C, uint32_t D, uint32_t T, uint32_t id) : C(C), D(D), T(T), id(id), prio(UINT32_MAX) {};
};

struct TaskJob {
    uint32_t arrival;
    uint32_t deadline;
    uint32_t instanceNumber;
    uint32_t taskId; // Index into the TaskSet
    uint32_t start;
    uint32_t end;

    TaskJob(uint32_t arrival, uint32_t deadline, uint32_t instanceNumber, uint32_t taskId) :
        arrival(arrival), deadline(deadline), instanceNumber(instanceNumber), taskId(taskId), start(UINT32_MAX), end(UINT32_MAX) {};

    void Print() const;
};