#include <array>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "PFairScheduler.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"

int8_t PFairScheduler::ComputeSign(uint32_t C, uint32_t T, uint32_t t) const {
    // We should compute: C/T(t+1) - floor(tC/T) - 1
    // But to keep full precision, we will multiply the equation with T and work with integers only:
    //      C(t+1) - T*floor(tC/T) - T
    // The floor function corresponds to rounding down to the closest multiple of T
    int32_t floorResult = C * t - (C * t) % T;
    int32_t result = C * (t + 1) - floorResult - T;

    // The sign is kept even if we multiplied by T
    if (result < 0) {
        return -1;
    }
    if (result > 0) {
        return 1;
    }
    return 0;
}

MultiprocessorSchedule PFairScheduler::GenerateScheduleImpl(TaskSet& taskSet, uint32_t numCpus) {
    MultiprocessorSchedule multiSchedule(taskSet, numCpus);

    // Shortcut to access all task ids in the task set
    std::vector<uint32_t> allTaskIds;
    for (auto &task : taskSet.GetTasks()) {
        allTaskIds.push_back(task.id);
    }

    uint32_t numTasks = taskSet.GetNumTasks();
    uint32_t hyperPeriod = taskSet.GetHyperPeriod();

    // The lag X period table, indexed first by time, then by task id
    std::vector<std::unordered_map<uint32_t, int32_t>> lagTable(hyperPeriod + 1);
    std::unordered_map<uint32_t, std::vector<int8_t>> charStrings(numTasks); // The characteristic strings for each task, indexed by task id

    // Set the first row to zero
    for (uint32_t taskId : allTaskIds) {
        lagTable[0][taskId] = 0;
    }

    // Compute characteristic strings
    for (uint32_t taskId : allTaskIds) {
        const PeriodicTask &task = taskSet.GetTask(taskId);

        // Reserve space for lower bound of string size
        charStrings[taskId] = std::vector<int8_t>();
        charStrings.at(taskId).reserve((std::size_t)hyperPeriod + 1);

        // Set an upper bound for t to prevent too long characteristic strings
        uint32_t max_t = hyperPeriod * 10;
        for (uint32_t t = 0; t < max_t; t++) {
            int8_t sign = ComputeSign(task.C, task.T, t);
            charStrings.at(taskId).push_back(sign);
            if (t >= hyperPeriod && sign == 0) {
                break; // We will never need more than up until the first zero after the interval we want to schedule
            }
        }
    }

    // Go through and fill up the lag X period table row by row
    for (uint32_t t = 0; t < hyperPeriod; t++) { // TODO: Set to hyperperiod or user max value
        // Step 1: Find Urgent, Tnegru and Contending tasks
        std::vector<uint32_t> urgent, tnegru, contending;
        for (uint32_t taskId : allTaskIds) {

            // Check if urgent
            if (lagTable[t].at(taskId) > 0 && charStrings.at(taskId)[t] != -1) {
                urgent.push_back(taskId);
                continue;
            }

            // Check if tnegru
            if (lagTable[t].at(taskId) < 0 && charStrings.at(taskId)[t] != 1) {
                tnegru.push_back(taskId);
                continue;
            }

            // Tasks that are not urgent or tnegru are contending
            contending.push_back(taskId);
        }

        // Step 2: Order contending tasks by lexicographical order of characteristic strings starting at t+1
        CharStringComparator comp(charStrings, t + 1); // Used to compare characteristic strings
        std::sort(contending.begin(), contending.end(), comp);

        // Step 3: Select tasks to allocate CPU resources
        std::vector<uint32_t> taskIdsToSchedule = urgent;
        taskIdsToSchedule.insert(taskIdsToSchedule.end(), contending.begin(), contending.end());
        
        if (taskIdsToSchedule.size() > numCpus) {
            // We can only schedule as many tasks as we have cores, so ignore the rest
            taskIdsToSchedule.resize(numCpus);
        }

        // Do the same thing to get the tasks we shouldn't schedule
        std::vector<uint32_t> taskIdsNotToSchedule = tnegru;
        taskIdsNotToSchedule.insert(taskIdsNotToSchedule.begin(), contending.begin(), contending.end());
        std::reverse(taskIdsNotToSchedule.begin(), taskIdsNotToSchedule.end());
        if (taskIdsNotToSchedule.size() > numTasks - numCpus) {
            taskIdsNotToSchedule.resize(numTasks - numCpus);
        }

        // Step 4: Schedule the selected tasks
        for (uint32_t cpuIdx = 0; cpuIdx < numCpus; cpuIdx++) {
            uint32_t taskId = taskIdsToSchedule[cpuIdx];
            const PeriodicTask &task = taskSet.GetTask(taskId);
            uint32_t instanceNumber = t / task.T;
            uint32_t arrival = instanceNumber * task.T;
            uint32_t deadline = (instanceNumber + 1) * task.T;
            uint32_t remainingTime = 0; // TODO: This is a dummy value. Actually compute it.
            uint32_t start = t, end = t + 1;
            TaskJob job(arrival, deadline, instanceNumber, taskId, remainingTime, start, end);
            multiSchedule.AddTaskJob(job, cpuIdx);
        }

        // Step 5: Update lag table values
        for (uint32_t taskId : taskIdsToSchedule) {
            // For each scheduled task, decrease table value by: period - execution time
            const PeriodicTask &task = taskSet.GetTask(taskId);
            lagTable[t + 1][taskId] = lagTable[t].at(taskId) - static_cast<int32_t>(task.T - task.C);
        }
        for (uint32_t taskId : taskIdsNotToSchedule) {
            // For each task not scheduled, increase table value by: execution time
            const PeriodicTask &task = taskSet.GetTask(taskId);
            lagTable[t + 1][taskId] = lagTable[t].at(taskId) + static_cast<int32_t>(task.C);
        }
        
        if (m_shouldPrintSteps) {
            PrintStep(t, allTaskIds, lagTable[t], urgent, tnegru, contending, taskIdsToSchedule);
        }
    }

    return multiSchedule;
}

void PFairScheduler::PrintStep(uint32_t t,
                               const std::vector<uint32_t> &allTaskIds,
                               const std::unordered_map<uint32_t, int32_t> &lagTableRow,
                               const std::vector<uint32_t> &urgent,
                               const std::vector<uint32_t> &tnegru,
                               const std::vector<uint32_t> &contending,
                               const std::vector<uint32_t> &toSchedule) const {

    // Helps print a list of task ids.
    auto PrintList = [](const std::string &label, const std::vector<uint32_t> &list, const std::string &end = "\n", const std::string &sep = " ") {
        std::cout << label;
        for (size_t i = 0; i < list.size(); ++i) {
            std::cout << list[i];
            if (i + 1 < list.size()) std::cout << sep;
        }
        std::cout << end;
    };

    std::cout << "Printing step t=" << t << ":" << std::endl;
    std::cout << "| Tasks:        ";
    for (uint32_t taskId : allTaskIds) {
        std::cout << "\t" << taskId;
    }
    std::cout << "\t|" << std::endl;
    
    std::cout << "| Lag * Period: ";
    for (uint32_t taskId : allTaskIds) {
        std::cout << "\t" << lagTableRow.at(taskId);
    }
    std::cout << "\t|" << std::endl;

    PrintList("Urgent: {", urgent, "}\n");
    PrintList("Tnegru: {", tnegru, "}\n");
    
    PrintList("Contending: ", contending, "\n", " > ");
    PrintList("To schedule: {", toSchedule, "}\n");
    
    std::cout << std::endl << std::endl;
}