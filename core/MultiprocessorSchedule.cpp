#include "MultiprocessorSchedule.hpp"

#include <iostream>
#include <stdexcept>

MultiprocessorSchedule::MultiprocessorSchedule(TaskSet taskSet, uint32_t numCpus) 
    : m_taskSet(taskSet), m_numCpus(numCpus) {
    m_cpuSchedules.resize(m_numCpus, UniprocessorSchedule(m_taskSet));
}

void MultiprocessorSchedule::AddTaskJob(TaskJob taskJob, uint32_t cpuIdx) {
    if (cpuIdx >= m_numCpus) {
        const std::string msg = "Cannot add task to cpu number " + std::to_string(cpuIdx + 1) + " when the multiprocessor schedule has only " + std::to_string(m_numCpus) + " CPUs.";
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
    return didFailForAnyCPU;
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