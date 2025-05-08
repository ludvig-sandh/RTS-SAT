#pragma once

#include "StaticPriorityScheduler.hpp"

class DMScheduler : public StaticPriorityScheduler {
public:
    void AssignStaticPriorities(TaskSet& taskSet) const override;
};