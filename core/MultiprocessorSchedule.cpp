#include "MultiprocessorSchedule.hpp"

#include <iostream>
#include <stdexcept>

MultiprocessorSchedule::MultiprocessorSchedule(TaskSet taskSet, uint32_t numCpus) 
    : BaseSchedule(taskSet), m_numCpus(numCpus) {
    m_cpuSchedules.resize(m_numCpus, UniprocessorSchedule(m_taskSet));
}

void MultiprocessorSchedule::AddTaskJob(TaskJob taskJob, uint32_t cpuIdx) {
    if (cpuIdx >= m_numCpus) {
        const std::string msg = "Cannot add task to cpu number " + std::to_string(cpuIdx + 1) + " when the multiprocessor schedule only has " + std::to_string(m_numCpus) + " CPUs.";
        throw std::invalid_argument(msg);
    }
    m_cpuSchedules[cpuIdx].AddTaskJob(taskJob);
}

bool MultiprocessorSchedule::AreDeadlinesMet(bool shouldPrintMiss) const {
    bool didFailForAnyCPU = false;
    for (uint32_t cpuIdx = 0; cpuIdx < m_numCpus; cpuIdx++) {
        bool cpuResult = m_cpuSchedules[cpuIdx].AreDeadlinesMet(shouldPrintMiss);
        didFailForAnyCPU |= cpuResult;
        if (!cpuResult && shouldPrintMiss) {
            std::cout << "(Deadlines missed in CPU number " << cpuIdx + 1 << "'s schedule)" << std::endl;
        }
    }
    if (!IsComplete()) {
        if (shouldPrintMiss) {
            std::cout << "(Deadlines missed because at least one task wasn't scheduled at all. Some scheduling algorithms stop when they cannot schedule a task within deadlines)" << std::endl;
        }

        return false;
    }
    return didFailForAnyCPU;
}

bool MultiprocessorSchedule::IsComplete() const {
    // Find union of all scheduled task ids
    std::unordered_set<std::string> scheduledIds;
    for (uint32_t cpuIdx = 0; cpuIdx < m_numCpus; cpuIdx++) {
        for (const TaskJob &job : m_cpuSchedules[cpuIdx].GetScheduledJobs()) {
            scheduledIds.insert(job.taskId);
        }
    }

    return (uint32_t)scheduledIds.size() == m_taskSet.GetNumTasks();
}

UniprocessorSchedule MultiprocessorSchedule::GetScheduleOfCore(uint32_t cpuIdx) {
    if (cpuIdx >= m_numCpus) {
        const std::string msg = "Cannot get schedule of cpu number " + std::to_string(cpuIdx + 1) + " when the multiprocessor schedule only has " + std::to_string(m_numCpus) + " CPUs.";
        throw std::invalid_argument(msg);
    }
    return m_cpuSchedules[cpuIdx];
}

void MultiprocessorSchedule::SetScheduleOfCore(UniprocessorSchedule schedule, uint32_t cpuIdx) {
    if (cpuIdx >= m_numCpus) {
        const std::string msg = "Cannot get schedule of cpu number " + std::to_string(cpuIdx + 1) + " when the multiprocessor schedule only has " + std::to_string(m_numCpus) + " CPUs.";
        throw std::invalid_argument(msg);
    }
    m_cpuSchedules[cpuIdx] = schedule;
}

void MultiprocessorSchedule::Validate() const {
    for (uint32_t cpuIdx = 0; cpuIdx < m_numCpus; cpuIdx++) {
        m_cpuSchedules[cpuIdx].Validate();
    }
}

void MultiprocessorSchedule::Print() const {
    std::cout << "Printing multiprocessor schedule:" << std::endl;
    for (uint32_t cpuIdx = 0; cpuIdx < m_numCpus; cpuIdx++) {
        std::cout << "Core " << cpuIdx + 1 << ":" << std::endl;
        m_cpuSchedules[cpuIdx].Print();
    }
}