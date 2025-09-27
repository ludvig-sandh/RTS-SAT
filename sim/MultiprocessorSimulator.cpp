#include "MultiprocessorSimulator.hpp"

MultiprocessorSchedule MultiprocessorSimulator::run(const TaskSet& taskSet) const {
    // Generate schedule and validate it before returning it
    MultiprocessorSchedule schedule = m_algorithm->GenerateSchedule(taskSet, m_config.numCpus);
    schedule.Validate();
    return schedule;
}