#include "MultiprocessorSchedulingAlgorithm.hpp"

MultiprocessorSchedule MultiprocessorSchedulingAlgorithm::GenerateSchedule(const TaskSet& taskSet, uint32_t numCpus) {
    // Generate schedule and validate it before returning it
    MultiprocessorSchedule schedule = GenerateScheduleImpl(taskSet, numCpus);
    schedule.Validate();
    return schedule;
}