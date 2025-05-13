#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include "Task.hpp"

class TaskSet {
private:
    std::vector<PeriodicTask> m_tasks;
public:
    TaskSet(std::vector<PeriodicTask> tasks);

    // Returns a reference to the vector of tasks
    const std::vector<PeriodicTask> &GetTasks() const;

    // Returns a copy of the vector of tasks
    std::vector<PeriodicTask> GetCopyOfTasks();

    // Returns a vector of all tasks with higher priority than prio
    std::vector<PeriodicTask> GetHigherPriorityTasks(uint32_t prio) const;

    // Sets the priority of a specific task
    void SetPriority(std::string taskId, uint32_t prio);
    const PeriodicTask &GetTask(std::string taskId) const;

    uint32_t GetNumTasks() const;
    uint32_t GetHyperPeriod() const;

    // Returns a vector of all task instances within the hyper period, sorted by ascending arrival time.
    std::deque<TaskJob> GetAllTaskJobs() const;

    // Returns a set of all absolute deadlines
    std::unordered_set<uint32_t> GetAbsoluteDeadlines() const;

    void PrintPriorities() const;
    void Print() const;

    // A task set is synchronous if and only if all tasks have the same offsets.
    bool IsSynchronous() const;

    // A task set has implicit deadlines if and only if all relative deadlines are equal to the relative periods.
    bool HasImplicitDeadlines() const;

    // A task set has constrained deadlines if and only if all relative deadlines are less than or equal to the relative periods.
    bool HasConstrainedDeadlines() const;

    // The utilization is the sum over each task execution time (C) divided by its period (T)
    double GetUtilization() const;
};