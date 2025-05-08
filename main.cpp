#include <iostream>
#include <numeric>

#include "Simulator.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "TaskSetGenerator.hpp"
#include "SchedulerBasedTests.hpp"
#include "ResponseTimeAnalysisTest.hpp"

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

TaskSet getTaskSet4() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(3, 5, 7, 1));
    tasks.push_back(PeriodicTask(2, 4, 14, 2));
    return TaskSet(tasks);
}

TaskSet getTaskSet5() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(2, 3, 3, 1));
    tasks.push_back(PeriodicTask(2, 5, 5, 2));
    return TaskSet(tasks);
}

TaskSet getTaskSet6() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(1, 4, 4, 1));
    tasks.push_back(PeriodicTask(4, 8, 8, 2));
    return TaskSet(tasks);
}

TaskSet getTaskSet7() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(2, 5, 5, 1));
    tasks.push_back(PeriodicTask(2, 6, 6, 2));
    return TaskSet(tasks);
}

bool testX(TaskSet &taskSet, uint32_t x) {
    PeriodicTask t1 = taskSet.GetTask(1);
    PeriodicTask t2 = taskSet.GetTask(2);
    if (t1.T > t2.T) {
        std::swap(t1, t2);
    }
    if (t1.T > t2.T) {
        exit(1);
    }

    // Response time of task 1
    if (t1.C > t1.D) {
        return false;
    }

    // Response time of task 2
    uint32_t estimatedResponseTime = t2.C;
    uint32_t lastEstimatedResponseTime = UINT32_MAX; // Placeholder value

    // Run iterative process until convergence
    while (estimatedResponseTime != lastEstimatedResponseTime && estimatedResponseTime <= t2.D) {
        lastEstimatedResponseTime = estimatedResponseTime;

        // Update the estimation
        // estimatedResponseTime = t2.C + ceil(static_cast<double>(lastEstimatedResponseTime) / t1.T) * (t1.C + x) - x;
        estimatedResponseTime = t2.C + ceil(static_cast<double>(lastEstimatedResponseTime) / t1.T) * (t1.C + x);
    }
    std::cout << "estimated response time: " << estimatedResponseTime << std::endl;

    return estimatedResponseTime <= t2.D;
}

void testRTA(TaskSet &taskSet) {
    // Check that our testX function and the sheduler based test always agree on schedulability
    bool schedulableResult = RMSchedulabilityTest(true).RunTest(taskSet);

    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    bool rtaResult = ResponseTimeAnalysisTest().RunTest(taskSet);

    if (schedulableResult != rtaResult) {
        std::cout << "THE FOLLOWING TASKSET FAILED" << std::endl;
        std::cout << "SCHEDULABLE RESULT: " << schedulableResult << ", OUR RESULT: " << rtaResult << std::endl;
        taskSet.Print();

        Simulator simulator(&rm); // Pass raw pointer since simulator doesn't take ownership.
        simulator.SetPreemptionsAllowed(true);

        Schedule schedule = simulator.run(taskSet);
        schedule.PrintSchedule();

        exit(0);
    }

    if (!schedulableResult) {
        return;
    }
}

void test(TaskSet &taskSet) {
    // Check that our testX function and the sheduler based test always agree on schedulability
    for (uint32_t x = 0; x < 100; x++) {
        bool schedulableResult = RMSchedulabilityTest(true, x).RunTest(taskSet);
        bool ourResult = testX(taskSet, x);

        if (schedulableResult != ourResult) {
            std::cout << "THE FOLLOWING TASKSET FAILED WITH x=" << x << std::endl;
            std::cout << "SCHEDULABLE RESULT: " << schedulableResult << ", OUR RESULT: " << ourResult << std::endl;
            taskSet.Print();

            RMScheduler alg;
            Simulator simulator(&alg); // Pass raw pointer since simulator doesn't take ownership.
            simulator.SetPreemptionsAllowed(true);
            simulator.SetPreemptionDelay(x);

            Schedule schedule = simulator.run(taskSet);
            schedule.PrintSchedule();

            exit(0);
        }

        if (!schedulableResult) {
            return;
        }
    }
}

int main() {
    // Configure the simulator
    // RMScheduler alg;
    // Simulator simulator(&alg);
    // simulator.SetPreemptionsAllowed(true);
    // simulator.SetPreemptionDelay(2);

    // // Create the task set
    // TaskSet taskSet = getTaskSet6();

    // // Run the algorithm to get the schedule
    // Schedule schedule = simulator.run(taskSet);

    // // Check deadlines
    // std::cout << "Checking if deadlines are respected..." << std::endl;
    // if (schedule.AreDeadlinesMet(true)) {
    //     std::cout << "No task missed their deadline.\n" << std::endl;
    // }

    // // Print information about the task priorities and schedule etc...
    // // taskSet.PrintPriorities();
    // schedule.PrintSchedule();

    // Test taskset generation
    TaskSetGenerator::Config config;
    config.minPeriod = 3;
    config.maxPeriod = 10;
    config.numTasks = 2;

    TaskSetGenerator generator(config);

    // TaskSet taskSet = getTaskSet7();
    // test(taskSet);

    for (uint32_t i = 0; i < 10; i++) {
        TaskSet taskSet = generator.Generate();

        test(taskSet);
    }

    std::cout << "SUCCESS!!!" << std::endl;
}