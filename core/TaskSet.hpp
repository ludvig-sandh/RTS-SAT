#pragma once

#include <vector>
#include <queue>
#include "Task.hpp"

class TaskSet {
private:
    std::vector<PeriodicTask> m_tasks;
public:
    TaskSet(std::vector<PeriodicTask> tasks) : m_tasks(tasks) {};
    const std::vector<PeriodicTask> &GetTasks() const;
    std::vector<PeriodicTask> GetCopyOfTasks();

    // Sets the priority of a specific task
    void SetPriority(uint32_t taskId, uint32_t prio);
    const PeriodicTask &GetTask(uint32_t taskId) const;

    std::size_t Size() const;
    uint32_t GetHyperPeriod() const;

    // Returns a vector of all task instances within the hyper period, sorted by ascending arrival time.
    std::deque<TaskJob> GetAllTaskJobs() const;
    void PrintPriorities() const;
};