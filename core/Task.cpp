#include <iostream>
#include "Task.hpp"

void TaskJob::Print() const {
    std::cout << "Printing TaskJob (from task #" << taskId << ", instance: " << instanceNumber << ")" << std::endl;
    std::cout << "\tarrival: " << arrival << std::endl;
    std::cout << "\tdeadline: " << deadline << std::endl;
    std::cout << "\tstart: " << start << std::endl;
    std::cout << "\tend: " << end << std::endl;
    std::cout << std::endl;
}