#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"

class EDFScheduler : public UniprocessorSchedulingAlgorithm {
public:
    UniprocessorSchedule GenerateSchedule(TaskSet &taskSet, bool allowPreemptions, uint32_t preemptionDelay) override;
};