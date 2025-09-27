#pragma once

#include "UniprocessorSchedulingAlgorithm.hpp"
#include "UniprocessorSchedule.hpp"

class UniprocessorSimulator {
public:
    struct Config {
        bool preemptionsAllowed;
        uint32_t preemptionDelay;
    };

    UniprocessorSimulator() = delete;
    explicit UniprocessorSimulator(UniprocessorSchedulingAlgorithm* const algo) : m_algorithm(algo), m_config(CreateDefaultConfig()) {}
    UniprocessorSimulator(UniprocessorSchedulingAlgorithm* const algo, Config config) : m_algorithm(algo), m_config(config) {}

    UniprocessorSchedule run(const TaskSet& taskSet) const;

    // Returns an instance of Config where preemptions are allowed and there's no preemption delay.
    static Config CreateDefaultConfig();

private:
    UniprocessorSchedulingAlgorithm* m_algorithm;
    Config m_config;
};