#pragma once

#include "TaskSet.hpp"

class BaseSchedule {
protected:
    TaskSet m_taskSet;
public:
    BaseSchedule(TaskSet taskSet) : m_taskSet(taskSet) {}

    virtual bool AreDeadlinesMet(bool shouldPrintMiss) const = 0;
    virtual void Validate() const = 0;
    virtual void Print() const = 0;
};