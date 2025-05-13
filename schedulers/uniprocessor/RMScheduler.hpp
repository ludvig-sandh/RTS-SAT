#pragma once

#include "StaticPriorityScheduler.hpp"

class RMScheduler : public StaticPriorityScheduler {
public:
    void AssignStaticPriorities(TaskSet& taskSet) const override;
};