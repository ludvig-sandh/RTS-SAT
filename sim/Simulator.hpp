#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"
#include "UniprocessorSchedule.hpp"

class Simulator {
    UniprocessorSchedulingAlgorithm* m_algorithm;
    bool m_isPreemptionsAllowed = true;
    uint32_t m_preemptionDelay = 0;

public:
    Simulator(UniprocessorSchedulingAlgorithm* algo) : m_algorithm(algo) {}

    void SetPreemptionsAllowed(bool isAllowed);
    void SetPreemptionDelay(uint32_t preemptionDelay);

    UniprocessorSchedule run(TaskSet taskSet) const;
};