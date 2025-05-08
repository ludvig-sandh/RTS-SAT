#include "Simulator.hpp"

void Simulator::SetPreemptionsAllowed(bool isAllowed) {
    m_isPreemptionsAllowed = isAllowed;
}

void Simulator::SetPreemptionDelay(uint32_t preemptionDelay) {
    m_preemptionDelay = preemptionDelay;
}

Schedule Simulator::run(TaskSet taskSet) const {
    Schedule schedule = m_algorithm->GenerateSchedule(taskSet, m_isPreemptionsAllowed, m_preemptionDelay);

    // Ensure that the schedule doesn't contain errors such as invalid times etc.
    schedule.Validate();
    
    return schedule;
}