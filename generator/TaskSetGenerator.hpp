#pragma once

#include <stdint.h>
#include "NumberGenerator.hpp"
#include "TaskSet.hpp"

class TaskSetGenerator {
public:
    struct Config {
        uint32_t numTasks;
        // double minUtilization = 0.1;
        // double maxUtilization = 0.9;
        bool implicitDeadlines = true; // D = T
        // bool allowOffsets = false;
        uint32_t minPeriod = 3;
        uint32_t maxPeriod = 30;
    };

    TaskSetGenerator(Config config);

    TaskSet Generate();

private:
    Config m_config;
    NumberGenerator m_rng;
};