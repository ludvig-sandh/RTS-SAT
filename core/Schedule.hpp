#pragma once

#include <stdint.h>
#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"

class Schedule {
private:
    TaskSet m_taskset;
    std::vector<TaskJob> m_scheduledTasks;
public:
    Schedule(TaskSet taskset) : m_taskset(taskset) {};
    void AddTaskJob(TaskJob taskFragment);
    bool AreDeadlinesMet(bool shouldPrintMiss);
    void PrintSchedule();
};