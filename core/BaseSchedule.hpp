// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include "TaskSet.hpp"

class BaseSchedule {
public:
    virtual bool AreDeadlinesMet(bool shouldPrintMiss) const = 0;

    // Checks that some invariants for the schedule and its tasks hold
    virtual void Validate() const = 0;

    virtual void Print() const = 0;

protected:
    TaskSet m_taskSet;
    BaseSchedule(const TaskSet& taskSet) : m_taskSet(taskSet) {}
};

class InvalidScheduleException : public std::exception {
public:
    // Constructor that accepts a custom error message
    InvalidScheduleException(const std::string& message) : errorMessage(message) {}

    // Adds more info to the error message regarding which schedule (CPU) didn't pass validation.
    void SpecifyCPU(uint32_t cpu_index) {
        cpuMessage = " (CPU: " + std::to_string(cpu_index) + ")";
    }

    // Override the what() method to return the custom error message
    const char* what() const noexcept override {
        return (errorMessage + cpuMessage).c_str();
    }

private:
    std::string errorMessage;
    std::string cpuMessage;
};