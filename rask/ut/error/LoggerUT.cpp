// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/error/Logger.hpp>
#include <gtest/gtest.h>

using namespace rask;

struct rask_error_Logger : testing::Test
{
    error::Logger logger;
};

TEST_F(rask_error_Logger, empty)
{
    ASSERT_TRUE(logger.errors().empty());
}

TEST_F(rask_error_Logger, twoMessages)
{
    error::Message msg1(Position("xxx", 3, 4), "Asia");
    error::Message msg2(Position("yyy", 7, 8), "Kasia");

    logger.log(msg1);
    logger.log(msg2);

    ASSERT_EQ(2u, logger.errors().size());
    ASSERT_EQ(msg1, logger.errors()[0]);
    ASSERT_EQ(msg2, logger.errors()[1]);
}

