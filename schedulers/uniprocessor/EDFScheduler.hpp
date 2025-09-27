// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"

class EDFScheduler : public UniprocessorSchedulingAlgorithm {
public:
    UniprocessorSchedule GenerateSchedule(const TaskSet& taskSet, bool allowPreemptions, uint32_t preemptionDelay) override;
};