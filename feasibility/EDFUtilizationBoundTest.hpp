// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once
#include "BaseFeasibilityTest.hpp"

// Exact test
class EDFUtilizationBoundTest : public BaseFeasibilityTest {
private:
    void CheckApplicability(const TaskSet& taskSet) const override;
    bool RunTestImpl(const TaskSet& taskSet) const override;
};