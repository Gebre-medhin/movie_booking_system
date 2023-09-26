/**
 * @file main_test.cpp
 * @brief Entry point for running Google Test (gtest) unit tests.
 *
 * This file provides the entry point for running unit tests written using the
 * Google Test (gtest) framework. It initializes the testing framework and runs
 * all the defined test cases.
 */

#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
