#include <gtest/gtest.h>

TEST(Sample, Addition) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_NE(2 + 2, 5);
}

TEST(Sample, Subtraction) {
    EXPECT_EQ(2 - 2, 0);
    EXPECT_NE(2 - 2, 1);
}

TEST(Test, Division) {
    EXPECT_EQ(2 / 2, 1);
    EXPECT_NE(2 / 2, 0);
}

TEST(Test, Multiplication) {
    EXPECT_EQ(2 * 2, 4);
    EXPECT_NE(2 * 2, 5);
}