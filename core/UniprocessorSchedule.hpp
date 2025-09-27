#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <exception>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "BaseSchedule.hpp"

class UniprocessorSchedule : public BaseSchedule {
public:
    using iterator = std::vector<TaskJob>::iterator;
    using const_iterator = std::vector<TaskJob>::const_iterator;

    iterator begin() { return m_scheduledJobs.begin(); }
    iterator end() { return m_scheduledJobs.end(); }
    const_iterator begin() const { return m_scheduledJobs.begin(); }
    const_iterator end() const { return m_scheduledJobs.end(); }

    explicit UniprocessorSchedule(const TaskSet& taskset) : BaseSchedule(taskset) {};
    void AddTaskJob(const TaskJob& taskJob);
    bool AreDeadlinesMet(bool shouldPrintMiss) const override;

    // Checks that the schedule is valid (i.e. no two tasks running simultaneously etc.)
    void Validate() const override;
    void Print() const override;
    void ExportToCsv(const std::string& filename = "schedule.csv") const;

private:
    std::vector<TaskJob> m_scheduledJobs;
};