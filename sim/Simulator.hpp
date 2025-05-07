#pragma once

#include "SchedulingAlgorithm.hpp"
#include "Schedule.hpp"

class Simulator {
    SchedulingAlgorithm* m_algorithm;
    bool m_isPreemptionsAllowed = true;

public:
    Simulator(SchedulingAlgorithm* algo) : m_algorithm(algo) {}

    void SetPreemptionsAllowed(bool isAllowed);

    Schedule run(TaskSet taskSet) const;
};