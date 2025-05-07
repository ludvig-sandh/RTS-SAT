#include <iostream>
#include <numeric>

#include "Simulator.hpp"
#include "RMScheduler.hpp"

TaskSet getTaskSet1() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(2, 3, 5, 1));
    tasks.push_back(PeriodicTask(5, 10, 10, 2));
    return TaskSet(tasks);
}

int main() {
    std::cout << "Hello world!\n" << std::endl;
    
    RMScheduler alg;
    Simulator simulator(&alg);
    TaskSet taskSet = getTaskSet1();
    Schedule schedule = simulator.run(taskSet);

    taskSet.PrintPriorities();
    schedule.PrintSchedule();
}