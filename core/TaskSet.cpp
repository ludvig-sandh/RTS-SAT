#include <numeric>
#include <algorithm>
#include <iostream>
#include "TaskSet.hpp"

TaskSet::TaskSet(std::vector<PeriodicTask> tasks) {
    if (tasks.size() == 0) {
        throw std::runtime_error("Cannot create a task set out of zero tasks.");
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

void TaskSet::SetPriority(uint32_t taskId, uint32_t prio) {
    for (PeriodicTask &task : m_tasks) {
        if (task.id == taskId) {
            task.prio = prio;
            return;
        }
    }
}

const PeriodicTask &TaskSet::GetTask(uint32_t taskId) const {
    for (const PeriodicTask &task : m_tasks) {
        if (task.id == taskId) {
            return task;
        }
    }
    // TODO: Raise exception
    return m_tasks.back();
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
                (instanceNumber - 1) * task.T, // absolute arrival
                (instanceNumber - 1) * task.T + task.D, // absolute deadline
                instanceNumber,
                (uint32_t)task.id,
                task.C // remaining time to execute
            );
            instances.push_back(instance);
        }
    }

    // Sort by arrival time (ascending) and then highest priority first
    std::sort(instances.begin(), instances.end(),
              [](const TaskJob& a, const TaskJob& b) {
                  return a.arrival < b.arrival;
              });

    return instances;
}

void TaskSet::PrintPriorities() const {
    std::cout << "Printing task priorities (higher number = higher priority)" << std::endl;
    for (const PeriodicTask &task : m_tasks) {
        std::cout << "Task id " << task.id << " has priority " << task.prio << std::endl;
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