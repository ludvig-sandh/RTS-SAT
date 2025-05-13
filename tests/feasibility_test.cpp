#include <gtest/gtest.h>

#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"
#include "LiuLaylandUtilizationBoundTest.hpp"
#include "EDFUtilizationBoundTest.hpp"
#include "EDFProcessorDemandTest.hpp"
#include "ResponseTimeAnalysisTest.hpp"
#include "RMScheduler.hpp"
#include "DMScheduler.hpp"

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


// Response Time Analysis (RTA) Tests

TEST(feasibility, RTA_ImplicitDeadline1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 4, 4, "A"),
        PeriodicTask(1, 5, 5, "B")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ImplicitDeadline2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(3, 5, 5, "A"),
        PeriodicTask(3, 5, 5, "B")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_FALSE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 4, "A"),
        PeriodicTask(1, 4, 5, "B"),
        PeriodicTask(1, 6, 6, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 3, 5, "A"),  // tight deadline
        PeriodicTask(2, 3, 6, "B"),  // tight deadline
        PeriodicTask(2, 4, 7, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_FALSE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline3) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 5, "A"),
        PeriodicTask(2, 6, 8, "B"),
        PeriodicTask(1, 9, 10, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline4) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 3, 5, "A"),
        PeriodicTask(3, 4, 6, "B"),
        PeriodicTask(2, 5, 7, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_FALSE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline5) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 2, 5, "A"),
        PeriodicTask(2, 5, 6, "B"),
        PeriodicTask(1, 9, 10, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_ConstrainedDeadline6) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 2, 5, "A"),
        PeriodicTask(3, 4, 6, "B"),
        PeriodicTask(1, 8, 10, "C")
    };
    TaskSet taskSet(tasks);
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}


// Response Time Analysis (DM) vs RM — Tasks where D != T

TEST(feasibility, RTA_DM_Passes_RM_Fails1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(3, 5, 7, "A"),
        PeriodicTask(2, 4, 14, "B"),
    };
    TaskSet taskSet(tasks);

    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_FALSE(ResponseTimeAnalysisTest().RunTest(taskSet));

    DMScheduler dm;
    dm.AssignStaticPriorities(taskSet);
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}

TEST(feasibility, RTA_DM_Passes_RM_Fails2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(2, 3, 6, "A"),
        PeriodicTask(1, 5, 5, "B"),
        PeriodicTask(2, 6, 6, "C")
    };
    TaskSet taskSet(tasks);
    
    RMScheduler rm;
    rm.AssignStaticPriorities(taskSet);
    EXPECT_FALSE(ResponseTimeAnalysisTest().RunTest(taskSet));

    DMScheduler dm;
    dm.AssignStaticPriorities(taskSet);
    // DM ordering: A > B > C
    // RM ordering: B > C > A (since B,C have shorter T)
    EXPECT_TRUE(ResponseTimeAnalysisTest().RunTest(taskSet));
}


// EDF CPU Demand Tests

TEST(feasibility, EDF_CPU_Demand1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(3, 5, 20, "1"),
        PeriodicTask(10, 25, 30, "2"),
        PeriodicTask(25, 40, 60, "3")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFProcessorDemandTest().RunTest(taskSet));
}

TEST(feasibility, EDF_CPU_Demand2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(6, 10, 15, "1"),
        PeriodicTask(2, 8, 25, "2"),
        PeriodicTask(20, 38, 75, "3")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(EDFProcessorDemandTest().RunTest(taskSet));
}

TEST(feasibility, EDF_CPU_Demand3) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 4, 4, "1"),
        PeriodicTask(3, 10, 15, "2"),
        PeriodicTask(6, 14, 18, "3")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(EDFProcessorDemandTest().RunTest(taskSet));
}