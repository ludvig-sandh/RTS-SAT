#pragma once

#include <vector>
#include <queue>
#include "Task.hpp"

class TaskSet {
private:
    std::vector<PeriodicTask> m_tasks;
public:
    TaskSet(std::vector<PeriodicTask> tasks);
    const std::vector<PeriodicTask> &GetTasks() const;
    std::vector<PeriodicTask> GetCopyOfTasks();

    // Sets the priority of a specific task
    void SetPriority(uint32_t taskId, uint32_t prio);
    const PeriodicTask &GetTask(uint32_t taskId) const;

    uint32_t GetNumTasks() const;
    uint32_t GetHyperPeriod() const;

    // Returns a vector of all task instances within the hyper period, sorted by ascending arrival time.
    std::deque<TaskJob> GetAllTaskJobs() const;
    void PrintPriorities() const;

    // A task set is synchronous if and only if all tasks have the same offsets.
    bool IsSynchronous() const;

    // A task set has implicit deadlines if and only if all relative deadlines are equal to the relative periods.
    bool HasImplicitDeadlines() const;
};