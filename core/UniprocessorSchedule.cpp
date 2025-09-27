#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "UniprocessorSchedule.hpp"
#include "Task.hpp"

void UniprocessorSchedule::AddTaskJob(const TaskJob& taskJob) {
    m_scheduledJobs.push_back(taskJob);
}

bool UniprocessorSchedule::AreDeadlinesMet(bool shouldPrintMiss) const {
    // Check if all jobs meet the deadlines of the tasks they belong to
    for (const TaskJob& taskJob : m_scheduledJobs) {
        if (taskJob.end <= taskJob.deadline) {
            continue; // Met its deadline
        }
        if (shouldPrintMiss) {
            std::cout << "The following task job didn't meet its deadline:\n";
            taskJob.Print();
        }
        return false;
    }
    return true;
}

void UniprocessorSchedule::Validate() const {
    uint32_t hyperPeriod = m_taskSet.GetHyperPeriod();
    
    if (hyperPeriod == 0) {
        throw InvalidScheduleException("Validation error: Hyperperiod is invalid (zero).");
    }
    
    const TaskJob *lastJob = nullptr;
    for (const TaskJob& job : m_scheduledJobs) {
        if (job.start > job.end) {
            throw InvalidScheduleException("Validation error: An instance of task " + job.taskId + " has a start time after its end time.");
        }
        if (lastJob != nullptr && job.end < lastJob->start) {
            throw InvalidScheduleException("Validation error: Schedule wasn't ordered by time");
        }
        if (lastJob != nullptr && job.start < lastJob->end) {
            throw InvalidScheduleException("Validation error: Instances of tasks " + job.taskId + " and " + lastJob->taskId + " executed at the same time.");
        }

        lastJob = &job;
    }
}

void UniprocessorSchedule::Print() const {
    std::cout << "Printing schedule" << "\n";
    for (const TaskJob& job : m_scheduledJobs) {
        std::cout << "Task " << job.taskId << " scheduled from " << job.start << " to " << job.end << " (instance " << job.instanceNumber << ")";
        if (job.end > job.deadline) {
            std::cout << " (missed deadline)";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void UniprocessorSchedule::ExportToCsv(const std::string& filename) const {
    std::ofstream file(filename);
    file << "arrival,deadline,instanceNumber,taskId,remainingTime,start,end,priority,executionTime,period,offset\n";
    for (const TaskJob& job : m_scheduledJobs) {
        file << std::to_string(job.arrival) << ",";
        file << std::to_string(job.deadline) << ",";
        file << std::to_string(job.instanceNumber) << ",";
        file << job.taskId << ",";
        file << std::to_string(job.remainingTime) << ",";
        file << std::to_string(job.start) << ",";
        file << std::to_string(job.end) << ",";
        file << std::to_string(m_taskSet.GetTask(job.taskId).prio) << ",";
        file << std::to_string(m_taskSet.GetTask(job.taskId).C) << ",";
        file << std::to_string(m_taskSet.GetTask(job.taskId).T) << ",";
        file << std::to_string(m_taskSet.GetTask(job.taskId).O) << "\n";
    }
    file.close();
    std::cout << "Wrote schedule to " << filename << "\n";
}