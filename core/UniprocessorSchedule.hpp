#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <exception>
#include "Task.hpp"
#include "TaskSet.hpp"

class UniprocessorSchedule {
private:
    TaskSet m_taskset;
    std::vector<TaskJob> m_scheduledTasks;
public:
    UniprocessorSchedule(TaskSet taskset) : m_taskset(taskset) {};
    void AddTaskJob(TaskJob taskJob);
    bool AreDeadlinesMet(bool shouldPrintMiss) const;

    // Checks that the schedule is valid (i.e. no two tasks running simultaneously etc.)
    void Validate() const;
    void Print() const;
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