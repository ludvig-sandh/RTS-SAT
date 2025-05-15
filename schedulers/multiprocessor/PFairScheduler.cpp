#include <array>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include "PFairScheduler.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"

namespace {
class ScheduleBuilder {
private:
    // The task set total utilization must not differ from m by more than this (m=num cpus)
    const double kUtilizationEpsilon = 0.000001;

    TaskSet &m_taskSet;
    uint32_t m_numCpus, m_hyperPeriod, m_numTasks;
    bool m_shouldPrintSteps;

    // The schedule to return by Build()
    MultiprocessorSchedule m_multiSchedule;

    // Shortcut to access all task ids in the task set
    std::vector<std::string> m_allTaskIds;
    
    // The lag X period table, indexed first by time, then by task id
    std::vector<std::unordered_map<std::string, int32_t>> m_lagTable;

    // The characteristic strings for each task, indexed by task id
    std::unordered_map<std::string, std::vector<int8_t>> m_charStrings;

public:
    ScheduleBuilder(TaskSet &taskSet, uint32_t numCpus, bool shouldPrintSteps = false)
        : m_taskSet(taskSet), m_numCpus(numCpus), m_shouldPrintSteps(shouldPrintSteps), m_multiSchedule(taskSet, numCpus) {}

    // Goes through all steps to create and return a P-Fair scheduling algorithm
    MultiprocessorSchedule Build() {
        Initialize();
        ComputeCharacteristicStrings();
        RunAlgorithm(); // Will schedule all tasks in the multiprocessor schedule
        return m_multiSchedule;
    }

private:
    // Computes the sign (-, 0 or +) in characteristic string at index t
    int8_t ComputeSign(uint32_t C, uint32_t T, uint32_t t) const {
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

    // Some preparation before the main algorithm can run
    void Initialize() {
        // Check requirements for the scheduling algorithm
        double utilization = m_taskSet.GetUtilization();
        if (std::abs(utilization - static_cast<double>(m_numCpus)) > kUtilizationEpsilon) {
            throw std::invalid_argument("P-Fair scheduling requires total utilization = number of CPUs.");
        }

        // Shortcut to access all task ids in the task set
        for (auto &task : m_taskSet.GetTasks()) {
            m_allTaskIds.push_back(task.id);
        }

        m_numTasks = m_taskSet.GetNumTasks();
        m_hyperPeriod = m_taskSet.GetHyperPeriod();

        m_lagTable.resize(m_hyperPeriod + 1);

        // Set the first row to zero
        for (std::string taskId : m_allTaskIds) {
            m_lagTable[0][taskId] = 0;
        }
    }

    // Compute the characteristic string for each task
    void ComputeCharacteristicStrings() {
        for (const PeriodicTask &task : m_taskSet.GetTasks()) {
            // Reserve space for lower bound of string size
            m_charStrings[task.id] = std::vector<int8_t>();
            m_charStrings.at(task.id).reserve((std::size_t)m_hyperPeriod + 1);

            if (m_shouldPrintSteps) {
                std::cout << "Characteristic string for task " << task.id << ":" << std::endl;
            }

            // Set an upper bound for t to prevent too long characteristic strings
            uint32_t max_t = m_hyperPeriod * 10;
            for (uint32_t t = 0; t < max_t; t++) {
                int8_t sign = ComputeSign(task.C, task.T, t);
                m_charStrings.at(task.id).push_back(sign);
                
                if (m_shouldPrintSteps) {
                    std::cout << (sign == 1 ? "+" : (sign == 0 ? "0" : "-"));
                }

                if (t >= m_hyperPeriod && sign == 0) {
                    break; // We will never need more than up until the first zero after the interval we want to schedule
                }
            }

            if (m_shouldPrintSteps) {
                std::cout << std::endl << std::endl;
            }
        }
    }

    // Go through and fill up the lag X period table row by row, and schedule the tasks at each time step
    void RunAlgorithm() {
        for (uint32_t t = 0; t < m_hyperPeriod; t++) {
            Step(t);
        }
    }

    // Performs step t of the P-Fair scheduling algorithm (schedules tasks at time t)
    void Step(uint32_t t) {
        // Step 1: Find Urgent, Tnegru and Contending tasks
        std::vector<std::string> urgent, tnegru, contending;
        for (std::string taskId : m_allTaskIds) {
            if (m_lagTable[t].at(taskId) > 0 && m_charStrings.at(taskId)[t] != -1) {
                urgent.push_back(taskId);
                continue;
            }

            if (m_lagTable[t].at(taskId) < 0 && m_charStrings.at(taskId)[t] != 1) {
                tnegru.push_back(taskId);
                continue;
            }

            contending.push_back(taskId);
        }

        // Step 2: Order contending tasks by lexicographical order of characteristic strings starting at t+1
        CharStringComparator comp(m_charStrings, t + 1); // Used to compare characteristic strings
        std::sort(contending.begin(), contending.end(), comp);

        // Step 3: Find which tasks to allocate CPU resources
        std::vector<std::string> taskIdsToSchedule = GetTasksToSchedule(urgent, contending);
        std::vector<std::string> taskIdsNotToSchedule = GetTasksNotToSchedule(tnegru, contending);
        assert(taskIdsToSchedule.size() + taskIdsNotToSchedule.size() == m_numTasks);
        
        // Step 4: Schedule the selected tasks
        ScheduleTasks(taskIdsToSchedule, t);

        // Step 5: Update lag table values
        FillNewLagTableRow(taskIdsToSchedule, taskIdsNotToSchedule, t);
        
        if (m_shouldPrintSteps) {
            PrintStep(t, urgent, tnegru, contending, taskIdsToSchedule);
        }
    }

    // Returns all tasks that should be scheduled
    std::vector<std::string> GetTasksToSchedule(std::vector<std::string> &urgent, std::vector<std::string> &contending) {
        std::vector<std::string> taskIdsToSchedule = urgent;
        taskIdsToSchedule.insert(taskIdsToSchedule.end(), contending.begin(), contending.end());
        
        if (taskIdsToSchedule.size() > m_numCpus) {
            // We can only schedule as many tasks as we have cores, so ignore the rest
            taskIdsToSchedule.resize(m_numCpus);
        }
        return taskIdsToSchedule;
    }

    // Returns all tasks that should not be scheduled
    std::vector<std::string> GetTasksNotToSchedule(std::vector<std::string> &tnegru, std::vector<std::string> &contending) {
        std::vector<std::string> taskIdsNotToSchedule = tnegru;
        taskIdsNotToSchedule.insert(taskIdsNotToSchedule.begin(), contending.begin(), contending.end());
        std::reverse(taskIdsNotToSchedule.begin(), taskIdsNotToSchedule.end());
        if (taskIdsNotToSchedule.size() > m_numTasks - m_numCpus) {
            taskIdsNotToSchedule.resize(m_numTasks - m_numCpus);
        }
        return taskIdsNotToSchedule;
    }

    // Creates the jobs and adds them to the multiprocessor schedule
    void ScheduleTasks(std::vector<std::string> &taskIdsToSchedule, uint32_t t) {
        for (uint32_t cpuIdx = 0; cpuIdx < m_numCpus; cpuIdx++) {
            std::string taskId = taskIdsToSchedule[cpuIdx];
            const PeriodicTask &task = m_taskSet.GetTask(taskId);
            uint32_t instanceNumber = t / task.T;
            uint32_t arrival = instanceNumber * task.T;
            uint32_t deadline = (instanceNumber + 1) * task.T;
            uint32_t remainingTime = 0; // TODO: This is a dummy value. Actually compute it.
            uint32_t start = t, end = t + 1;
            TaskJob job(arrival, deadline, instanceNumber, taskId, remainingTime, start, end);
            m_multiSchedule.AddTaskJob(job, cpuIdx);
        }
    }

    // Fills the lag table at time t + 1 (next row)
    void FillNewLagTableRow(std::vector<std::string> &taskIdsToSchedule, std::vector<std::string> &taskIdsNotToSchedule, uint32_t t) {
        for (std::string taskId : taskIdsToSchedule) {
            // For each scheduled task, decrease table value by: period - execution time
            const PeriodicTask &task = m_taskSet.GetTask(taskId);
            m_lagTable[t + 1][taskId] = m_lagTable[t].at(taskId) - static_cast<int32_t>(task.T - task.C);
        }
        for (std::string taskId : taskIdsNotToSchedule) {
            // For each task not scheduled, increase table value by: execution time
            const PeriodicTask &task = m_taskSet.GetTask(taskId);
            m_lagTable[t + 1][taskId] = m_lagTable[t].at(taskId) + static_cast<int32_t>(task.C);
        }
    }

    // Displays information about timestep t in the console.
    void PrintStep(uint32_t t,
                   const std::vector<std::string> &urgent,
                   const std::vector<std::string> &tnegru,
                   const std::vector<std::string> &contending,
                   const std::vector<std::string> &toSchedule) const {

        // Helps print a list of task ids.
        auto PrintList = [](const std::string &label, const std::vector<std::string> &list, const std::string &end = "\n", const std::string &sep = " ") {
            std::cout << label;
            for (size_t i = 0; i < list.size(); ++i) {
                std::cout << list[i];
                if (i + 1 < list.size()) std::cout << sep;
            }
            std::cout << end;
        };

        std::cout << "Printing step t=" << t << ":" << std::endl;
        std::cout << "| Tasks:        ";
        for (std::string taskId : m_allTaskIds) {
            std::cout << "\t" << taskId;
        }
        std::cout << "\t|" << std::endl;
        
        std::cout << "| Lag * Period: ";
        for (std::string taskId : m_allTaskIds) {
            std::cout << "\t" << m_lagTable[t].at(taskId);
        }
        std::cout << "\t|" << std::endl;

        PrintList("Urgent: {", urgent, "}\n");
        PrintList("Tnegru: {", tnegru, "}\n");
        
        PrintList("Contending: ", contending, "\n", " > ");
        PrintList("To schedule: {", toSchedule, "}\n");
        
        std::cout << std::endl << std::endl;
    }
};
} // unnamed namespace

MultiprocessorSchedule PFairScheduler::GenerateScheduleImpl(TaskSet &taskSet, uint32_t numCpus) {
    ScheduleBuilder builder(taskSet, numCpus, m_shouldPrintSteps);
    return builder.Build();
}