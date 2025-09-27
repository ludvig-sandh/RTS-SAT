// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Ludvig Sandh

#pragma once

#include <string>
#include "TaskSet.hpp"

class BaseFeasibilityTest {
public:
    virtual ~BaseFeasibilityTest() = default;

    bool RunTest(const TaskSet& taskSet);

protected:
    // Checks if this feasibility test is applicable to the given task set, and throws an exception otherwise
    virtual void CheckApplicability(const TaskSet& taskSet) const = 0;

    // Runs the feasibility test on a given task set, given that it is applicable
    virtual bool RunTestImpl(const TaskSet& taskSet) const = 0;
};

class InvalidFeasibilityTestException : public std::exception {
public:
    // Constructor that accepts a custom error message
    InvalidFeasibilityTestException(const std::string& message)
        : errorMessage(message) {}

    // Override the what() method to return the custom error message
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

    static InvalidFeasibilityTestException CreateNonSynchronousException();
    static InvalidFeasibilityTestException CreateNonImplicitDeadlinesException();
    static InvalidFeasibilityTestException CreateNonConstrainedDeadlinesException();

private:
    std::string errorMessage;
};