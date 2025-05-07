#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <exception>
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

    // Checks that the schedule is valid (i.e. no two tasks running simultaneously etc.)
    void Validate();
    void PrintSchedule();
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