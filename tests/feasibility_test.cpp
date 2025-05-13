#include <gtest/gtest.h>

#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"
#include "EDFUtilizationBoundTest.hpp"

// Liu & Layland's Utilization Bound Tests

TEST(feasibility, LiuLayland1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 4, "A"),
        PeriodicTask(1, 5, "B")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 5, "A"),
        PeriodicTask(3, 6, "B")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland3) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, "A"),
        PeriodicTask(1, 4, "B"),
        PeriodicTask(1, 6, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland4) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 5, "A"),
        PeriodicTask(2, 6, "B"),
        PeriodicTask(2, 7, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland5) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 5, "A"),
        PeriodicTask(1, 6, "B"),
        PeriodicTask(1, 7, "C"),
        PeriodicTask(1, 8, "D")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland6) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 5, "A"),
        PeriodicTask(2, 6, "B"),
        PeriodicTask(2, 7, "C"),
        PeriodicTask(2, 8, "D")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland7_JustOverBound) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 5, "A"),
        PeriodicTask(1, 6, "B"),
        PeriodicTask(1, 7, "C"),
        PeriodicTask(1, 4, "D")
    }; // Total U = ~0.7595 (n=4, bound ≈ 0.7568)

    TaskSet taskSet(tasks);
    EXPECT_FALSE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, LiuLayland8_JustUnderBound) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, "A"),
        PeriodicTask(2, 6, "B"),
        PeriodicTask(1, 11, "C")
    }; // Total U = ~0.758 (n=3, bound = ~0.7798)

    TaskSet taskSet(tasks);
    EXPECT_TRUE(LiuLaylandUtilizationBoundTest().RunTest(taskSet));
}


// EDF Utilization Bound Tests

TEST(feasibility, EDF1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 4, "A"),
        PeriodicTask(1, 5, "B")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(3, 5, "A"),   // 0.6
        PeriodicTask(3, 5, "B")    // 0.6 → total = 1.2
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF3) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, "A"),   // 0.333
        PeriodicTask(1, 4, "B"),   // 0.25
        PeriodicTask(1, 6, "C")    // 0.167 → total ≈ 0.75
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF4) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 5, "A"),   // 0.4
        PeriodicTask(2, 6, "B"),   // 0.333
        PeriodicTask(2, 7, "C")    // 0.285 → total ≈ 1.018
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF5) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 5, "A"),   // 0.2
        PeriodicTask(1, 6, "B"),   // 0.167
        PeriodicTask(1, 7, "C"),   // 0.143
        PeriodicTask(1, 8, "D")    // 0.125 → total ≈ 0.635
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF6) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 5, "A"),   // 0.4
        PeriodicTask(2, 6, "B"),   // 0.333
        PeriodicTask(2, 7, "C"),   // 0.285
        PeriodicTask(2, 8, "D")    // 0.25 → total ≈ 1.27
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF7_JustUnderBound) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 2, "A"),   // 0.5
        PeriodicTask(2, 5, "B"),   // 0.4
        PeriodicTask(1, 20, "C")   // 0.05 → total = 0.95
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(EDFUtilizationBoundTest().RunTest(taskSet));
}

TEST(feasibility, EDF8_JustOverBound) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 2, "A"),   // 0.5
        PeriodicTask(3, 5, "B"),   // 0.6
        PeriodicTask(1, 20, "C")   // 0.05 → total = 1.15
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFUtilizationBoundTest().RunTest(taskSet));
}
