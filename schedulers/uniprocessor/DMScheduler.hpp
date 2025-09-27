// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include "StaticPriorityScheduler.hpp"

class DMScheduler : public StaticPriorityScheduler {
public:
    void AssignStaticPriorities(TaskSet& taskSet) const override;
};