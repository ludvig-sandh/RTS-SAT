#pragma once

#include <vector>
#include "TaskSet.hpp"
#include "UniprocessorSchedule.hpp"
#include "BaseSchedule.hpp"

class MultiprocessorSchedule : public BaseSchedule {
private:
    uint32_t m_numCpus;

    // Stores a list of single core schedules, indexed by core id
    std::vector<UniprocessorSchedule> m_cpuSchedules;

public:
    MultiprocessorSchedule(TaskSet taskSet, uint32_t numCpus);
    void AddTaskJob(TaskJob taskJob, uint32_t cpuIdx);
    bool AreDeadlinesMet(bool shouldPrintMiss) const override;

    // Checks if all tasks were actually scheduled.
    // In case of some scheduling algorithms (such as RMFF), tasks are scheduled one by one until they can't, which might leave tasks unscheduled.
    bool IsComplete() const;

    // Getter and setter for the uniprocessor schedule for a core index (0-indexed)
    UniprocessorSchedule GetScheduleOfCore(uint32_t cpuIdx);
    void SetScheduleOfCore(UniprocessorSchedule schedule, uint32_t cpuIdx);

    void Validate() const override;
    void Print() const override;
};