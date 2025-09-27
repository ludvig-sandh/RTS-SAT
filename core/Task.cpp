// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#include <iostream>
#include "Task.hpp"

PeriodicTask::PeriodicTask(uint32_t C, uint32_t T, const std::string& id)
    : C(C), D(T), T(T), O(0), id(id), prio(UINT32_MAX) {
    CheckNonZeroPeriod();
};

PeriodicTask::PeriodicTask(uint32_t C, uint32_t D, uint32_t T, const std::string& id)
    : C(C), D(D), T(T), O(0), id(id), prio(UINT32_MAX) {
    CheckNonZeroPeriod();
};

PeriodicTask::PeriodicTask(uint32_t C, uint32_t D, uint32_t T, uint32_t O, const std::string& id)
    : C(C), D(D), T(T), O(O), id(id), prio(UINT32_MAX) {
    CheckNonZeroPeriod();
};

void PeriodicTask::CheckNonZeroPeriod() const {
    if (T == 0) {
        throw std::invalid_argument("Cannot create instance of PeriodicTask with zero period.");
    }
}

void PrintPeriodicTask(const PeriodicTask& task) {
    std::cout << "Task id " << task.id << ", C=" << task.C << ", D=" << task.D << ", T=" << task.T << "\n";
}

void TaskJob::Print() const {
    std::cout << "Printing TaskJob (from task #" << taskId << ", instance: " << instanceNumber << ")" << "\n";
    std::cout << "\tarrival: " << arrival << "\n";
    std::cout << "\tdeadline: " << deadline << "\n";
    std::cout << "\tstart: " << start << "\n";
    std::cout << "\tend: " << end << "\n";
    std::cout << std::endl;
}