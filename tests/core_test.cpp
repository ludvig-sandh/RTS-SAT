#include <gtest/gtest.h>

#include <vector>
#include "Task.hpp"
#include "TaskSet.hpp"

TEST(core, NumTasks) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "3"),
        PeriodicTask(1, 1, 7, "7"),
        PeriodicTask(1, 1, 5, "5")
    };
    TaskSet taskSet(tasks);
    EXPECT_EQ(taskSet.GetNumTasks(), (uint32_t)3);
}

TEST(core, GetTask) {
    std::string name = "helloo";
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "3"),
        PeriodicTask(1, 1, 7, name),
        PeriodicTask(1, 1, 5, "5")
    };
    TaskSet taskSet(tasks);
    EXPECT_EQ(taskSet.GetTask(name).id, name);
}

TEST(core, SetPriority) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "ABC")
    };
    TaskSet taskSet(tasks);
    taskSet.SetPriority("ABC", 15);
    EXPECT_EQ(taskSet.GetTask("ABC").prio, (uint32_t)15);
}

TEST(core, EmptyTaskSet) {
    std::vector<PeriodicTask> tasks;
    EXPECT_THROW(TaskSet taskSet(tasks), std::invalid_argument);
}

TEST(core, HyperPeriod1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "3"),
        PeriodicTask(1, 1, 7, "7"),
        PeriodicTask(1, 1, 5, "5")
    };
    TaskSet taskSet(tasks);
    EXPECT_EQ(taskSet.GetHyperPeriod(), (uint32_t)3 * 7 * 5);
}

TEST(core, HyperPeriod2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 2, "2"),
        PeriodicTask(1, 1, 4, "4"),
        PeriodicTask(1, 1, 8, "8"),
        PeriodicTask(1, 1, 16, "16"),
        PeriodicTask(1, 1, 32, "32"),
        PeriodicTask(1, 1, 64, "64"),
        PeriodicTask(1, 1, 128, "128"),
        PeriodicTask(1, 1, 3, "3")
    };
    TaskSet taskSet(tasks);
    EXPECT_EQ(taskSet.GetHyperPeriod(), (uint32_t)384);
}

TEST(core, HyperPeriod3) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 6, "6"),
        PeriodicTask(1, 1, 1, "1"),
        PeriodicTask(1, 1, 6, "6")
    };
    TaskSet taskSet(tasks);
    EXPECT_EQ(taskSet.GetHyperPeriod(), (uint32_t)6);
}

TEST(core, ZeroPeriodTask) {
    EXPECT_THROW(PeriodicTask(0, 0, 0, "0"), std::invalid_argument);
}

TEST(core, Synchronous) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 5, "A"),
        PeriodicTask(1, 5, 7, "B"),
        PeriodicTask(2, 7, 10, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(taskSet.IsSynchronous());
}

TEST(core, NotSynchronous) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 5, "A"),
        PeriodicTask(1, 5, 7, "B"),
        PeriodicTask(2, 7, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(taskSet.IsSynchronous());
}

TEST(core, ImplicitDeadlines) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 5, 5, "A"),
        PeriodicTask(1, 7, 7, "B"),
        PeriodicTask(2, 10, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(taskSet.HasImplicitDeadlines());
}

TEST(core, NotImplicitDeadlines) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 3, 5, "A"),
        PeriodicTask(1, 5, 7, "B"),
        PeriodicTask(2, 7, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(taskSet.HasImplicitDeadlines());
}

TEST(core, ConstrainedDeadlines) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 5, 5, "A"),
        PeriodicTask(1, 7, 7, "B"),
        PeriodicTask(2, 10, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_TRUE(taskSet.HasConstrainedDeadlines());
}

TEST(core, NotConstrainedDeadlines) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 6, 5, "A"),
        PeriodicTask(1, 5, 7, "B"),
        PeriodicTask(2, 7, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_FALSE(taskSet.HasConstrainedDeadlines());
}

TEST(core, Utilization1) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 6, 5, "A"),
        PeriodicTask(1, 5, 7, "B"),
        PeriodicTask(2, 7, 10, 2, "C")
    };
    TaskSet taskSet(tasks);
    EXPECT_DOUBLE_EQ(taskSet.GetUtilization(), 1.0 / 5.0 + 1.0 / 7.0 + 2.0 / 10.0);
}

TEST(core, Utilization2) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 6, 2, "A"),
        PeriodicTask(1, 5, 4, "B"),
        PeriodicTask(1, 7, 8, 2, "C"),
        PeriodicTask(1, 7, 8, 2, "D")
    };
    TaskSet taskSet(tasks);
    EXPECT_DOUBLE_EQ(taskSet.GetUtilization(), 1.0);
}

TEST(core, ZeroUtilization) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(0, 6, 2, "A")
    };
    TaskSet taskSet(tasks);
    EXPECT_DOUBLE_EQ(taskSet.GetUtilization(), 0.0);
}

TEST(core, AbsoluteDeadlines) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "A"),
        PeriodicTask(1, 3, 4, "B"),
        PeriodicTask(1, 4, 6, "C")
    };
    TaskSet taskSet(tasks);

    std::unordered_set<uint32_t> deadlinesExpected = {
        1, 4, 7, 10, 3, 11
    };

    std::unordered_set<uint32_t> deadlinesActual = taskSet.GetAbsoluteDeadlines();

    EXPECT_EQ(deadlinesExpected.size(), deadlinesActual.size());

    for (uint32_t deadlineExpected : deadlinesExpected) {
        EXPECT_TRUE(deadlinesActual.find(deadlineExpected) != deadlinesActual.end());
    }
}

TEST(core, AllTaskJobs) {
    std::vector<PeriodicTask> tasks = {
        PeriodicTask(1, 1, 3, "A"),
        PeriodicTask(1, 3, 4, "B"),
        PeriodicTask(1, 4, 6, "C")
    };
    TaskSet taskSet(tasks);

    std::deque<TaskJob> jobs = taskSet.GetAllTaskJobs();

    EXPECT_EQ(jobs.size(), (std::size_t)9);
}