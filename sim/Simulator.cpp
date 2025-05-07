#include "Simulator.hpp"

void Simulator::SetPreemptionsAllowed(bool isAllowed) {
    m_isPreemptionsAllowed = isAllowed;
}

Schedule Simulator::run(TaskSet taskSet) const {
    Schedule schedule = m_algorithm->GenerateSchedule(taskSet, m_isPreemptionsAllowed);

    // Ensure that the schedule doesn't contain errors such as invalid times etc.
    schedule.Validate();
    
    return schedule;
}