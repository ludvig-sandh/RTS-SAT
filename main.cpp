#include <iostream>
#include <numeric>
#include <string>

#include "Simulator.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"
#include "EDFScheduler.hpp"
#include "TaskSetGenerator.hpp"
#include "SchedulerBasedTests.hpp"
#include "ResponseTimeAnalysisTest.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"
#include "PFairScheduler.hpp"

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
    Simulator simulator(&scheduler);
    simulator.SetPreemptionsAllowed(true);

    // Select a task set
    TaskSet taskSet = getSampleTaskSet1();
    taskSet.Print(); // Display task set in the terminal

    // Get the schedule by running the simulator
    UniprocessorSchedule schedule = simulator.run(taskSet);
    
    // Print the scheduled jobs in the terminal
    schedule.Print();

    // Check deadlines
    if (schedule.AreDeadlinesMet(true)) { // Since we send true here, this function will print any missed tasks
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
    Simulator simulator(&scheduler);
    simulator.SetPreemptionsAllowed(true);
    
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

void example4() {
    std::vector<PeriodicTask> tasks;

    // Tasks from paper:
    // tasks.push_back(PeriodicTask(1, 3, 3, "v"));
    // tasks.push_back(PeriodicTask(2, 4, 4, "w"));
    // tasks.push_back(PeriodicTask(5, 7, 7, "x"));
    // tasks.push_back(PeriodicTask(8, 11, 11, "y"));
    // tasks.push_back(PeriodicTask(335, 462, 462, "z"));

    // Tasks from HW2:
    tasks.push_back(PeriodicTask(4, 8, 8, "1"));
    tasks.push_back(PeriodicTask(1, 4, 4, "2"));
    tasks.push_back(PeriodicTask(7, 8, 8, "3"));
    tasks.push_back(PeriodicTask(3, 8, 8, "4"));

    TaskSet taskSet(tasks);

    PFairScheduler scheduler(true);
    MultiprocessorSchedule schedule = scheduler.GenerateSchedule(taskSet, 2);
}

int main() {
    // example1();
    // example2();
    // example3();
    example4();
}