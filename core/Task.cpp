#include <iostream>
#include "Task.hpp"

PeriodicTask::PeriodicTask(uint32_t C, uint32_t D, uint32_t T, const std::string &id)
    : C(C), D(D), T(T), O(0), id(id), prio(UINT32_MAX) {
    CheckNonZeroPeriod();
};

PeriodicTask::PeriodicTask(uint32_t C, uint32_t D, uint32_t T, uint32_t O, const std::string &id)
    : C(C), D(D), T(T), O(O), id(id), prio(UINT32_MAX) {
    CheckNonZeroPeriod();
};

void PeriodicTask::CheckNonZeroPeriod() const {
    if (T == 0) {
        throw std::invalid_argument("Cannot create instance of PeriodicTask with zero period.");
    }
}

void TaskJob::Print() const {
    std::cout << "Printing TaskJob (from task #" << taskId << ", instance: " << instanceNumber << ")" << std::endl;
    std::cout << "\tarrival: " << arrival << std::endl;
    std::cout << "\tdeadline: " << deadline << std::endl;
    std::cout << "\tstart: " << start << std::endl;
    std::cout << "\tend: " << end << std::endl;
    std::cout << std::endl;
}