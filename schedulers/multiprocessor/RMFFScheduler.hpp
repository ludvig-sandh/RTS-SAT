#pragma once

#include <unordered_map>
#include "MultiprocessorSchedulingAlgorithm.hpp"

class RMFFScheduler : public MultiprocessorSchedulingAlgorithm {
protected:
    MultiprocessorSchedule GenerateScheduleImpl(TaskSet &taskSet, uint32_t numCpus) override;
};