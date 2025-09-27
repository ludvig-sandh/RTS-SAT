#include <numeric>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>
#include "TaskSet.hpp"

TaskSet TaskSet::GetHigherPriorityTasks(uint32_t prio) const {
    auto filtered_view = m_tasks
        | std::views::filter([prio](const PeriodicTask& task) {
            return task.prio > prio;
        });

    std::vector<PeriodicTask> tasks(filtered_view.begin(), filtered_view.end());
    return TaskSet(tasks);
}

void TaskSet::SetPriority(const std::string& taskId, uint32_t prio) {
    // Avoid code duplication by calling const helper function.
    // Since this is a non-const member function, it is safe to cast away constness.
    PeriodicTask& foundTask = const_cast<PeriodicTask&>(GetTask(taskId));
    foundTask.prio = prio;
}

const PeriodicTask& TaskSet::GetTask(const std::string& taskId) const {
    auto match = [&taskId](const PeriodicTask& task) {
        return task.id == taskId;
    };
    auto it = std::ranges::find_if(m_tasks, match);
    if (it != m_tasks.end()) {
        return *it;
    }
    
    throw std::invalid_argument("Could not find task with id " + taskId + " in the task set.");
}

size_t TaskSet::GetNumTasks() const {
    return m_tasks.size();
}

uint32_t TaskSet::GetHyperPeriod() const {
    if (m_tasks.empty()) {
        throw std::domain_error("Cannot compute hyperperiod from en empty task set.");
    }
    uint32_t current_lcm = 1;
    for (const PeriodicTask& task : m_tasks) {
        current_lcm = std::lcm(current_lcm, task.T);
    }
    return current_lcm;
}

std::deque<TaskJob> TaskSet::GetAllTaskJobs() const {
    std::deque<TaskJob> instances;

    // Safety guard for empty task set
    if (m_tasks.empty()) {
        return instances;
    }

    uint32_t hyperPeriod = GetHyperPeriod();

    // For each task, generate all the instances within the hyperperiod
    for (const PeriodicTask& task : m_tasks) {
        uint32_t numInstances = hyperPeriod / task.T;
        for (uint32_t instanceNumber = 1; instanceNumber < numInstances + 1; instanceNumber++) {
            TaskJob instance(
                task.O + (instanceNumber - 1) * task.T, // absolute arrival
                task.O + (instanceNumber - 1) * task.T + task.D, // absolute deadline
                instanceNumber,
                task.id,
                task.C // remaining time to execute
            );
            instances.push_back(instance);
        }
    }

    // Sort by arrival time (ascending)
    std::sort(instances.begin(), instances.end(),
        [](const TaskJob& a, const TaskJob& b) {
            return a.arrival < b.arrival;
        });

    return instances;
}

std::unordered_set<uint32_t> TaskSet::GetAbsoluteDeadlines() const {
    std::unordered_set<uint32_t> deadlines;

    // Safety guard for empty task set
    if (m_tasks.empty()) {
        return deadlines;
    }

    uint32_t hyperPeriod = GetHyperPeriod();

    // For each task, generate all absolute deadlines
    for (const PeriodicTask& task : m_tasks) {
        uint32_t numInstances = hyperPeriod / task.T;
        for (uint32_t instanceNumber = 1; instanceNumber < numInstances + 1; instanceNumber++) {
            deadlines.insert(task.O + (instanceNumber - 1) * task.T + task.D); // absolute deadline
        }
    }

    return deadlines;
}

void TaskSet::PrintPriorities() const {
    if (m_tasks.empty()) {
        std::cout << "Printing task priorities (higher number = higher priority)\n\tEmpty task set.\n";
        return;
    }
    std::cout << "Printing task priorities (higher number = higher priority)\n";
    for (const PeriodicTask& task : m_tasks) {
        std::cout << "Task id " << task.id << " has priority " << task.prio << "\n";
    }
    std::cout << "\n";
}

void TaskSet::Print() const {
    if (m_tasks.empty()) {
        std::cout << "Printing task set (higher number = higher priority)\n\tEmpty task set.\n";
        return;
    }
    std::cout << "Printing task set\n";
    for (const PeriodicTask& task : m_tasks) {
        PrintPeriodicTask(task);
    }
    std::cout << "\n";
}

bool TaskSet::IsSynchronous() const {
    // Only compute the result once
    if (m_cachedIsSynchronous.has_value()) {
        return *m_cachedIsSynchronous;
    }

    bool isSynchronous = true;
    for (uint32_t i = 1; i < GetNumTasks(); i++) {
        if (m_tasks[i].O != m_tasks[0].O) {
            isSynchronous = false;
            break;
        }
    }

    m_cachedIsSynchronous = isSynchronous;
    return isSynchronous;
}

bool TaskSet::HasImplicitDeadlines() const {
    if (m_cachedHasImplicitDeadlines.has_value()) {
        return *m_cachedHasImplicitDeadlines;
    }

    bool result = true;
    for (const PeriodicTask& task : m_tasks) {
        if (task.D != task.T) {
            result = false;
            break;
        }
    }

    m_cachedHasImplicitDeadlines = result;
    return result;
}

bool TaskSet::HasConstrainedDeadlines() const {
    if (m_cachedHasConstrainedDeadlines.has_value()) {
        return *m_cachedHasConstrainedDeadlines;
    }

    bool result = true;
    for (const PeriodicTask& task : m_tasks) {
        if (task.D > task.T) {
            result = false;
            break;
        }
    }
    
    m_cachedHasConstrainedDeadlines = result;
    return result;
}

double TaskSet::GetUtilization() const {
    if (m_cachedUtilization.has_value()) {
        return *m_cachedUtilization;
    }

    double utilization = 0.0;
    for (const PeriodicTask& task : m_tasks) {
        utilization += static_cast<double>(task.C) / static_cast<double>(task.T);
    }

    m_cachedUtilization = utilization;
    return utilization;
}