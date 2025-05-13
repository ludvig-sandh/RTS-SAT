#pragma once

#include "MultiprocessorSchedulingAlgorithm.hpp"

class PFairScheduler : public MultiprocessorSchedulingAlgorithm {
protected:
    MultiprocessorSchedule GenerateScheduleImpl(TaskSet& taskSet, uint32_t numCpus) override;
};