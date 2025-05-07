#pragma once

#include "SchedulingAlgorithm.hpp"
#include "Schedule.hpp"

class Simulator {
    SchedulingAlgorithm* m_algorithm;

public:
    Simulator(SchedulingAlgorithm* algo) : m_algorithm(algo) {}

    Schedule run(TaskSet& taskSet) {
        Schedule schedule = m_algorithm->GenerateSchedule(taskSet);
        return schedule;
    }
};