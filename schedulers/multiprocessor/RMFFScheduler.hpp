// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <unordered_map>
#include "MultiprocessorSchedulingAlgorithm.hpp"

class RMFFScheduler : public MultiprocessorSchedulingAlgorithm {
protected:
    MultiprocessorSchedule GenerateScheduleImpl(const TaskSet& taskSet, uint32_t numCpus) override;
};