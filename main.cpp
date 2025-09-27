// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#include <iostream>
#include <numeric>
#include <string>
#include <cassert>

#include "UniprocessorSimulator.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "TaskSetGenerator.hpp"
#include "UniprocessorSchedulerBasedTest.hpp"
#include "MultiprocessorSchedulerBasedTest.hpp"
#include "ResponseTimeAnalysisTest.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"
#include "PFairScheduler.hpp"
#include "RMFFScheduler.hpp"
#include "RMFFUtilizationBoundTest.hpp"
#include "MultiprocessorSchedule.hpp"

// Returns a sample task set
TaskSet getSampleTaskSet1() {
    std::vector<PeriodicTask> tasks;
    tasks.push_back(PeriodicTask(2, 5, 5, "1"));
    tasks.push_back(PeriodicTask(3, 10, 10, "2"));
    return TaskSet(tasks);
}

// Schedules the sample task set with RM
void example1() {
    // Let's select RM
    RMScheduler scheduler;

    // Configure the simulator
    UniprocessorSimulator simulator(&scheduler);

    // Select a task set
    TaskSet taskSet = getSampleTaskSet1();
    taskSet.Print(); // Display task set

    // Get the schedule by running the simulator
    UniprocessorSchedule schedule = simulator.run(taskSet);
    
    // Print the scheduled jobs
    schedule.Print();

    // Check deadlines
    std::cout << "Checking if deadlines are respected..." << std::endl;
    if (schedule.AreDeadlinesMet(true)) { // Since we provide true value here, this function will print any missed tasks
        std::cout << "No task missed their deadline.\n" << std::endl;
    }

    // Export schedule to csv file (so the python script can visualize it)
    schedule.ExportToCsv();
}

// Schedules a randomized task set with DM
void example2() {
    // Let's select DM
    DMScheduler scheduler;

    // Configure the simulator
    UniprocessorSimulator simulator(&scheduler);
    
    // Create a configuration for the task set generator
    TaskSetGenerator::Config config;
    config.minPeriod = 3;
    config.maxPeriod = 6;
    config.numTasks = 4;
    
    // Use the config to generate a random task set
    TaskSetGenerator generator(config);
    TaskSet taskSet = generator.Generate();

    // Get the schedule by running the simulator
    UniprocessorSchedule schedule = simulator.run(taskSet);
    
    // Check deadlines
    std::cout << "Checking if deadlines are respected..." << std::endl;
    if (schedule.AreDeadlinesMet(true)) {
        std::cout << "No task missed their deadline.\n" << std::endl;
    }
    
    // Print the scheduled jobs in the terminal, and export them to a csv file (so the python script can visualize it)
    schedule.Print();
    schedule.ExportToCsv();
}

// Runs a Liu & Layland's feasibility test on the sample task set 
void example3() {
    // Get task set
    TaskSet taskSet = getSampleTaskSet1();

    // Display the task set in the terminal, as well as its utilization
    taskSet.Print(); 
    std::cout << "The task set has utilization U=" << taskSet.GetUtilization() << std::endl;

    // Create the feasibility test and run it on the task set
    LiuLaylandUtilizationBoundTest feasibilityTest;
    bool didSucceed = feasibilityTest.RunTest(taskSet);

    std::string resultString = didSucceed ? "succeeded" : "failed";
    std::cout << "Liu & Layland's utilization bound test " << resultString << std::endl;
}

// Simulates a P-Fair scheduling algorithm on a set of periodic tasks
void example4() {
    // Example tasks from P-Fair scheduling paper:
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 3, "v"),
        PeriodicTask(2, 4, 4, "w"),
        PeriodicTask(5, 7, 7, "x"),
        PeriodicTask(8, 11, 11, "y"),
        PeriodicTask(335, 462, 462, "z")
    };
    TaskSet taskSet(tasks);
    
    // Specify number of cpu:s used in the simulator
    MultiprocessorSimulator::Config simConfig;
    simConfig.numCpus = 2;
    
    // Run the scheduler
    PFairScheduler scheduler(true); // shouldPrintSteps = true
    MultiprocessorSimulator simulator(&scheduler, simConfig);
    MultiprocessorSchedule schedule = simulator.run(taskSet);
}

void example5() {
    std::vector<PeriodicTask> tasks;

    // Tasks from HW2 (problem 6)
    tasks.push_back(PeriodicTask(40, 157, "t1"));
    tasks.push_back(PeriodicTask(21, 21, "t2"));
    tasks.push_back(PeriodicTask(14, 50, "t3"));
    tasks.push_back(PeriodicTask(39, 159, "t4"));
    tasks.push_back(PeriodicTask(10, 49, "t5"));
    tasks.push_back(PeriodicTask(37, 40, "t6"));

    TaskSet taskSet(tasks);

    RMFFScheduler scheduler;
    uint32_t numCpus = 3;
    MultiprocessorSchedule schedule = scheduler.GenerateSchedule(taskSet, numCpus);

    std::cout << schedule.AreDeadlinesMet(true) << std::endl;
    std::cout << RMFFSchedulabilityTest(numCpus).RunTest(taskSet) << std::endl;
    std::cout << RMFFUtilizationBoundTest(numCpus).RunTest(taskSet) << std::endl;
}

int main() {
    example1();
    example2();
    example3();
    example4();
    example5();
}