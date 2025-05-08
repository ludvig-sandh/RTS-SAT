#pragma once

#include "SchedulingAlgorithm.hpp"

class EDFScheduler : public SchedulingAlgorithm {
public:
    Schedule GenerateSchedule(TaskSet &taskSet, bool allowPreemptions, uint32_t preemptionDelay) override;
};