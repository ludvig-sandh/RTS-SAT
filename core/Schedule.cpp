#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Schedule.hpp"
#include "Task.hpp"

void Schedule::AddTaskJob(TaskJob taskJob) {
    m_scheduledTasks.push_back(taskJob);
}

bool Schedule::AreDeadlinesMet(bool shouldPrintMiss) {
    // Check if all fragments meet the deadlines of the tasks they belong to
    for (const TaskJob &taskJob : m_scheduledTasks) {
        if (taskJob.end > taskJob.deadline) {
            if (shouldPrintMiss) {
                std::cout << "The following task job didn't meet its deadline:" << std::endl;
                taskJob.Print();
            }
            return false;
        }
    }
    
    return true;
}

void Schedule::Validate() const {
    const TaskJob *lastJob = nullptr;
    uint32_t hyperPeriod = m_taskset.GetHyperPeriod();

    if (hyperPeriod == 0) {
        throw InvalidScheduleException("Validation error: Hyperperiod is invalid (zero).");
    }

    for (const TaskJob &job : m_scheduledTasks) {
        if (job.start > job.end) {
            throw InvalidScheduleException("Validation error: An instance of task " + std::to_string(job.taskId) + " has a start time after its end time.");
        }
        if (lastJob && job.end < lastJob->start) {
            throw InvalidScheduleException("Validation error: Schedule wasn't ordered by time");
        }
        if (lastJob && job.start < lastJob->end) {
            throw InvalidScheduleException("Validation error: Instances of tasks " + std::to_string(job.taskId) + " and " + std::to_string(lastJob->taskId) + " executed at the same time.");
        }

        lastJob = &job;
    }
}

void Schedule::Print() const {
    std::cout << "Printing schedule" << std::endl;
    for (const TaskJob &job : m_scheduledTasks) {
        std::cout << "Task " << job.taskId << " scheduled from " << job.start << " to " << job.end << " (instance " << job.instanceNumber << ")";
        if (job.end > job.deadline) {
            std::cout << " (missed deadline)";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Schedule::ExportToCsv(const std::string& filename) const {
    std::ofstream file(filename);
    file << "arrival,deadline,instanceNumber,taskId,remainingTime,start,end,priority,executionTime,period,offset\n";
    for (const TaskJob &job : m_scheduledTasks) {
        file << std::to_string(job.arrival) << ",";
        file << std::to_string(job.deadline) << ",";
        file << std::to_string(job.instanceNumber) << ",";
        file << std::to_string(job.taskId) << ",";
        file << std::to_string(job.remainingTime) << ",";
        file << std::to_string(job.start) << ",";
        file << std::to_string(job.end) << ",";
        file << std::to_string(m_taskset.GetTask(job.taskId).prio) << ",";
        file << std::to_string(m_taskset.GetTask(job.taskId).C) << ",";
        file << std::to_string(m_taskset.GetTask(job.taskId).T) << ",";
        file << std::to_string(m_taskset.GetTask(job.taskId).O) << "\n";
    }
    file.close();
    std::cout << "Wrote schedule to " << filename << "\n";
}