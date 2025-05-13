#pragma once

#include <unordered_map>
#include "MultiprocessorSchedulingAlgorithm.hpp"

class PFairScheduler : public MultiprocessorSchedulingAlgorithm {
private:
    bool m_shouldPrintSteps;
public:
    PFairScheduler(bool shouldPrintSteps = false) : m_shouldPrintSteps(shouldPrintSteps) {};
protected:
    MultiprocessorSchedule GenerateScheduleImpl(TaskSet &taskSet, uint32_t numCpus) override;

private:
    int8_t ComputeSign(uint32_t C, uint32_t T, uint32_t t) const;

    // Prints debug info for scheduling step t, including task IDs, lag values,
    // and categorized task lists like urgent, tnegru, contending, etc.
    void PrintStep(uint32_t t,
                   const std::vector<std::string> &allTaskIds,
                   const std::unordered_map<std::string, int32_t> &lagTableRow,
                   const std::vector<std::string> &urgent,
                   const std::vector<std::string> &tnegru,
                   const std::vector<std::string> &contending,
                   const std::vector<std::string> &toSchedule) const;
};

struct CharStringComparator {
    const std::unordered_map<std::string, std::vector<int8_t>> &charString;
    const uint32_t t;

    CharStringComparator(const std::unordered_map<std::string, std::vector<int8_t>> &charString, uint32_t t) : charString(charString), t(t) {}

    // Should return true if a should come before b
    bool operator()(std::string a, std::string b) const {
        std::size_t i = t;

        // Use at() instead of [] operator because it is const and won't modify the map
        while (i < charString.at(a).size() && i < charString.at(b).size()) {
            // Return true if a has larger string lexicographically than b (then a will come before b)
            int8_t aChar = charString.at(a)[i], bChar = charString.at(b)[i];
            if (aChar > bChar) {
                return true; // a is definitely larger than b
            }
            if (aChar < bChar) {
                return false; // a is definitely smaller than b
            }

            // They had the same value, so check next character
            i++;
        }

        // Resolve ties using lowest index
        return a < b;
    }
};