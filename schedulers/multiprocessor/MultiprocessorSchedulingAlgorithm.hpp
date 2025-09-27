// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <vector>
#include <stdexcept>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "MultiprocessorSchedule.hpp"

class MultiprocessorSchedulingAlgorithm {
public:
    // Core method each algorithm must implement
    MultiprocessorSchedule GenerateSchedule(const TaskSet& taskSet, uint32_t numCpus);

protected:
    virtual MultiprocessorSchedule GenerateScheduleImpl(const TaskSet& taskSet, uint32_t numCpus) = 0;
};