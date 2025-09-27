#include "TaskSetGenerator.hpp"

TaskSetGenerator::TaskSetGenerator(Config config) : m_config(config) {}

TaskSet TaskSetGenerator::Generate() {
    std::vector<PeriodicTask> tasks;
    for (uint32_t i = 0; i < m_config.numTasks; i++) {
        uint32_t T = m_rng.GenerateInRange(m_config.minPeriod, m_config.maxPeriod);
        uint32_t C = m_rng.GenerateInRange(1, T * 2 / 3);
        uint32_t D = T;
        if (!m_config.implicitDeadlines) {
            D = m_rng.GenerateInRange(C, T);
        }
        PeriodicTask task(C, D, T, std::to_string(i + 1));
        tasks.push_back(task);
    }
    return TaskSet(tasks);
}