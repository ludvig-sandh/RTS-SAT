#pragma once

#include "SchedulingAlgorithm.hpp"
#include "Schedule.hpp"

class Simulator {
    SchedulingAlgorithm* m_algorithm;
    bool m_isPreemptionsAllowed = true;
    uint32_t m_preemptionDelay = 0;

public:
    Simulator(SchedulingAlgorithm* algo) : m_algorithm(algo) {}

    void SetPreemptionsAllowed(bool isAllowed);
    void SetPreemptionDelay(uint32_t preemptionDelay);

    Schedule run(TaskSet taskSet) const;
};