#pragma once

#include "SchedulingAlgorithm.hpp"

class RMScheduler : public SchedulingAlgorithm {
public:
    Schedule GenerateSchedule(TaskSet &taskSet) override;
private:
    void AssignStaticPriorities(TaskSet& taskSet);
};