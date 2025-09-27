#include "UniprocessorSimulator.hpp"

UniprocessorSchedule UniprocessorSimulator::run(const TaskSet& taskSet) const {
    // Generate schedule and validate it before returning it
    UniprocessorSchedule schedule = m_algorithm->GenerateSchedule(taskSet, m_config.preemptionsAllowed, m_config.preemptionDelay);
    schedule.Validate();
    return schedule;
}

UniprocessorSimulator::Config UniprocessorSimulator::CreateDefaultConfig() {
    UniprocessorSimulator::Config c;
    c.preemptionsAllowed = true;
    c.preemptionDelay = 0;
    return c;
}