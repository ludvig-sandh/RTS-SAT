#pragma once

#include "StaticPriorityScheduler.hpp"

class DMScheduler : public StaticPriorityScheduler {
private:
    void AssignStaticPriorities(TaskSet& taskSet) override;
};