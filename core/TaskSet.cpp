#include <numeric>
#include <algorithm>
#include <iostream>
#include "TaskSet.hpp"

TaskSet::TaskSet(std::vector<PeriodicTask> tasks) {
    if (tasks.size() == 0) {
        throw std::invalid_argument("Cannot create a task set out of zero tasks.");
    }

    m_tasks = tasks;
}

const std::vector<PeriodicTask> &TaskSet::GetTasks() const {
    return m_tasks;
}

std::vector<PeriodicTask> TaskSet::GetCopyOfTasks() {
    return m_tasks;
}

std::vector<PeriodicTask> TaskSet::GetHigherPriorityTasks(uint32_t prio) const {
    std::vector<PeriodicTask> tasks;
    for (const PeriodicTask &task : m_tasks) {
        if (task.prio > prio) {
            tasks.push_back(task);
        }
    }
    return tasks;
}

void TaskSet::SetPriority(std::string taskId, uint32_t prio) {
    for (PeriodicTask &task : m_tasks) {
        if (task.id == taskId) {
            task.prio = prio;
            return;
        }
    }
}

const PeriodicTask &TaskSet::GetTask(std::string taskId) const {
    for (const PeriodicTask &task : m_tasks) {
        if (task.id == taskId) {
            return task;
        }
    }
    
    throw std::invalid_argument("Cannot run GetTask() with a taskId that doesn't exist in the task set.");
}

uint32_t TaskSet::GetNumTasks() const {
    return static_cast<uint32_t>(m_tasks.size());
}

uint32_t TaskSet::GetHyperPeriod() const {
    uint32_t current_lcm = 1;
    for (const PeriodicTask &task : m_tasks) {
        current_lcm = std::lcm(current_lcm, task.T);
    }
    return current_lcm;
}

std::deque<TaskJob> TaskSet::GetAllTaskJobs() const {
    std::deque<TaskJob> instances;
    uint32_t hyperPeriod = GetHyperPeriod();

    // For each task, generate all the instances within the hyperperiod
    for (const PeriodicTask &task : m_tasks) {
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
    uint32_t hyperPeriod = GetHyperPeriod();

    // For each task, generate all absolute deadlines
    for (const PeriodicTask &task : m_tasks) {
        uint32_t numInstances = hyperPeriod / task.T;
        for (uint32_t instanceNumber = 1; instanceNumber < numInstances + 1; instanceNumber++) {
            deadlines.insert(task.O + (instanceNumber - 1) * task.T + task.D); // absolute deadline
        }
    }

    return deadlines;
}

void TaskSet::PrintPriorities() const {
    std::cout << "Printing task priorities (higher number = higher priority)" << std::endl;
    for (const PeriodicTask &task : m_tasks) {
        std::cout << "Task id " << task.id << " has priority " << task.prio << std::endl;
    }
    std::cout << std::endl;
}

void TaskSet::Print() const {
    std::cout << "Printing task set" << std::endl;
    for (const PeriodicTask &task : m_tasks) {
        std::cout << "Task id " << task.id << ", C=" << task.C << ", D=" << task.D << ", T=" << task.T << std::endl;
    }
    std::cout << std::endl;
}

bool TaskSet::IsSynchronous() const {
    for (uint32_t i = 1; i < GetNumTasks(); i++) {
        if (m_tasks[i].O != m_tasks[0].O) {
            return false;
        }
    }
    return true;
}

bool TaskSet::HasImplicitDeadlines() const {
    for (const PeriodicTask &task : m_tasks) {
        if (task.D != task.T) {
            return false;
        }
    }
    return true;
}

bool TaskSet::HasConstrainedDeadlines() const {
    for (const PeriodicTask &task : m_tasks) {
        if (task.D > task.T) {
            return false;
        }
    }
    return true;
}

double TaskSet::GetUtilization() const {
    double utilization = 0.0;
    for (const PeriodicTask &task : m_tasks) {
        utilization += static_cast<double>(task.C) / static_cast<double>(task.T);
    }
    return utilization;
}