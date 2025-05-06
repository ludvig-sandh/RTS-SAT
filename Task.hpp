#pragma once

#include <stdint.h>

enum Priority {
    HHH,
    HHL,
    HLL,
    LHH,
    LLH,
    LLL
};

typedef struct {
    uint32_t C; // WCET
    uint32_t T; // Period
    uint32_t D; // Deadline
    Priority prio;
} Task;

typedef struct {
    uint32_t start;
    uint32_t end;
    Priority prio;
} TaskFragment;