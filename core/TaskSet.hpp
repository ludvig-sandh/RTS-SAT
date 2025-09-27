#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include <optional>
#include "Task.hpp"

class TaskSet {
public:
    using iterator = std::vector<PeriodicTask>::iterator;
    using const_iterator = std::vector<PeriodicTask>::const_iterator;

    iterator begin() { return m_tasks.begin(); }
    iterator end() { return m_tasks.end(); }
    const_iterator begin() const { return m_tasks.begin(); }
    const_iterator end() const { return m_tasks.end(); }

    TaskSet() = delete;
    TaskSet(const std::vector<PeriodicTask>& tasks) : m_tasks(tasks) {}
    TaskSet(const TaskSet& other) = default;
    TaskSet(TaskSet&& other) = default;
    TaskSet& operator=(const TaskSet& other) = default;
    TaskSet& operator=(TaskSet&& other) = default;

    // Returns a vector of all tasks with higher priority than prio
    TaskSet GetHigherPriorityTasks(uint32_t prio) const;

    // Sets the priority of a specific task
    void SetPriority(const std::string& taskId, uint32_t prio);

    // Gets a specific task
    const PeriodicTask& GetTask(const std::string& taskId) const;

    size_t GetNumTasks() const;
    uint32_t GetHyperPeriod() const;

    // Returns a deque of all task instances within the hyper period, sorted by ascending arrival time.
    std::deque<TaskJob> GetAllTaskJobs() const;

    // Returns a set of all absolute deadlines
    std::unordered_set<uint32_t> GetAbsoluteDeadlines() const;

    // Prints priorities for all tasks in the set
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

private:
    std::vector<PeriodicTask> m_tasks;

    // Used to cache results from some member functions
    mutable std::optional<bool> m_cachedIsSynchronous = std::nullopt;
    mutable std::optional<bool> m_cachedHasImplicitDeadlines = std::nullopt;
    mutable std::optional<bool> m_cachedHasConstrainedDeadlines = std::nullopt;
    mutable std::optional<double> m_cachedUtilization = std::nullopt;
};