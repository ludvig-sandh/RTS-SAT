// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include "MultiprocessorSchedulingAlgorithm.hpp"
#include "MultiprocessorSchedule.hpp"

class MultiprocessorSimulator {
public:
    struct Config {
        uint32_t numCpus;

        Config() : numCpus(1) {}
        Config(uint32_t numCpus) : numCpus(numCpus) {}
    };

    MultiprocessorSimulator() = delete;
    MultiprocessorSimulator(MultiprocessorSchedulingAlgorithm* const algo, Config config) : m_algorithm(algo), m_config(config) {}

    MultiprocessorSchedule run(const TaskSet& taskSet) const;

private:
    MultiprocessorSchedulingAlgorithm* m_algorithm;
    Config m_config;
};