#include <iostream>
#include <numeric>

#include "Simulator.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"

TaskSet getTaskSet1() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(2, 3, 5, 1));
    tasks.push_back(PeriodicTask(5, 10, 10, 2));
    return TaskSet(tasks);
}

TaskSet getTaskSet2() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(1, 3, 5, 1));
    tasks.push_back(PeriodicTask(5, 6, 10, 2));
    return TaskSet(tasks);
}

TaskSet getTaskSet3() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(1, 2, 10, 1));
    tasks.push_back(PeriodicTask(2, 4, 5, 2));
    return TaskSet(tasks);
}

int main() {
    std::cout << "Hello world!\n" << std::endl;
    
    RMScheduler alg;
    Simulator simulator(&alg);
    TaskSet taskSet = getTaskSet3();
    Schedule schedule = simulator.run(taskSet);
    std::cout << "Checking if deadlines are respected..." << std::endl;
    if (schedule.AreDeadlinesMet(true)) {
        std::cout << "No task missed their deadline.\n" << std::endl;
    }

    taskSet.PrintPriorities();
    schedule.PrintSchedule();
}