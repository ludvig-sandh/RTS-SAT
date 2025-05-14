#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <exception>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "BaseSchedule.hpp"

class UniprocessorSchedule : public BaseSchedule {
private:
    std::vector<TaskJob> m_scheduledJobs;
public:
    UniprocessorSchedule(TaskSet taskset) : BaseSchedule(taskset) {};
    void AddTaskJob(TaskJob taskJob);
    bool AreDeadlinesMet(bool shouldPrintMiss) const override;

    // Returns a reference to the scheduled tasks
    const std::vector<TaskJob> &GetScheduledJobs() const;

    // Checks that the schedule is valid (i.e. no two tasks running simultaneously etc.)
    void Validate() const override;
    void Print() const override;
    void ExportToCsv(const std::string& filename = "schedule.csv") const;
};

class InvalidScheduleException : public std::exception {
private:
    std::string errorMessage;

public:
    // Constructor that accepts a custom error message
    InvalidScheduleException(const std::string& message)
        : errorMessage(message) {}

    // Override the what() method to return the custom error message
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};