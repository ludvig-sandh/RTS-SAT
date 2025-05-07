#include <assert.h>
#include <iostream>
#include "Schedule.hpp"
#include "Task.hpp"

void Schedule::AddTaskJob(TaskJob taskJob) {
    m_scheduledTasks.push_back(taskJob);
}

bool Schedule::AreDeadlinesMet() {
    // Check if all fragments meet the deadlines of the tasks they belong to
    for (const TaskJob &taskJob : m_scheduledTasks) {
        if (taskJob.end > taskJob.deadline) {
            return false;
        }
    }
    
    return true;
}

void Schedule::PrintSchedule() {
    std::cout << "Printing schedule" << std::endl;
    for (const TaskJob &job : m_scheduledTasks) {
        std::cout << "Task " << job.taskId << " scheduled from " << job.start << " to " << job.end << " (instance " << job.instanceNumber << ")" << std::endl;
    }
    std::cout << std::endl;
}