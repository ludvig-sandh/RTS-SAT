#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include "Task.hpp"

class Schedule {
private:
    std::unordered_map<Priority, Task> m_taskset;
    std::vector<TaskFragment> m_scheduledTasks;
public:
    Schedule(std::vector<Task> taskset);
    void AddTaskFragment(TaskFragment taskFragment);
    bool AreDeadlinesMet();
};