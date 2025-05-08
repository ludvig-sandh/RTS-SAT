#include <stdexcept>
#include "NumberGenerator.hpp"

uint32_t NumberGenerator::GenerateInRange(uint32_t low, uint32_t high) {
    if (low > high) {
        throw std::invalid_argument("Invalid arguments for random number generator range: \'low\' cannot be larger than \'high\'.");
    }
    return m_rng() % (high - low + 1) + low;
}