#include <stdexcept>
#include "FeasibilityTest.hpp"

bool FeasibilityTest::RunTest(const TaskSet& taskSet) {
    if (!IsApplicable(taskSet)) {
        throw std::runtime_error("Feasibility test is not applicable to the given task set.");
    }
    return RunTestImpl(taskSet);
}