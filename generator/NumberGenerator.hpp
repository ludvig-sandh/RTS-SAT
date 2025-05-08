#pragma once

#include <random>
#include <stdint.h>

class NumberGenerator {
private:
    std::mt19937 m_rng;
public:
    // Generate an integer in the range [low, high] (inclusive)
    uint32_t GenerateInRange(uint32_t low, uint32_t high);
};