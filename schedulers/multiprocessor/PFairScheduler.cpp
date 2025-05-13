#include "PFairScheduler.hpp"

#include "RMScheduler.hpp"
#include "DMScheduler.hpp"

MultiprocessorSchedule PFairScheduler::GenerateScheduleImpl(TaskSet& taskSet, uint32_t numCpus) {
    MultiprocessorSchedule multiSchedule(taskSet, numCpus);

    // TODO: Remove dummy code
    for (uint32_t i = 0; i < numCpus; i++) {
        if (i == 0) {
            UniprocessorSchedule uniSchedule = RMScheduler().GenerateSchedule(taskSet, true, 0);
            multiSchedule.SetScheduleOfCore(uniSchedule, i);
        }else {
            UniprocessorSchedule uniSchedule = DMScheduler().GenerateSchedule(taskSet, true, 0);
            multiSchedule.SetScheduleOfCore(uniSchedule, i);
        }
    }

    return multiSchedule;
}