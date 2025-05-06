#include <assert.h>
#include "Schedule.hpp"
#include "Task.hpp"

Schedule::Schedule(std::vector<Task> taskset) {
    for (const Task &task : taskset) {
        m_taskset[task.prio] = task;
    }
}

void Schedule::AddTaskFragment(TaskFragment taskFragment) {
    m_scheduledTasks.push_back(taskFragment);
}

bool Schedule::AreDeadlinesMet() {
    // Check if all fragments meet the deadlines of the tasks they belong to
    for (const TaskFragment &taskFragment : m_scheduledTasks) {
        assert(m_taskset.find(taskFragment.prio) != m_taskset.end() && "Found task fragment with priority that doesn't exist in the task set.");
        Task *task = &m_taskset[taskFragment.prio];
        if (taskFragment.end > task->D) {
            return false;
        }
    }

    return true;
}