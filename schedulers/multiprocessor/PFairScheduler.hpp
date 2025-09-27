// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <unordered_map>
#include "MultiprocessorSchedulingAlgorithm.hpp"

class PFairScheduler : public MultiprocessorSchedulingAlgorithm {
private:
    bool m_shouldPrintSteps;
public:
    PFairScheduler(bool shouldPrintSteps = false) : m_shouldPrintSteps(shouldPrintSteps) {};
protected:
    MultiprocessorSchedule GenerateScheduleImpl(const TaskSet& taskSet, uint32_t numCpus) override;
};

struct CharStringComparator {
    const std::unordered_map<std::string, std::vector<int8_t>>& charString;
    const uint32_t t;

    CharStringComparator(const std::unordered_map<std::string, std::vector<int8_t>>& charString, uint32_t t) : charString(charString), t(t) {}

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