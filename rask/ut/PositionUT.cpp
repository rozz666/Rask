// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/Position.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace rask;

struct rask_Position : testing::Test
{
    void assertString(std::string str, Position pos)
    {
        std::ostringstream ss;
        ss << pos;
        ASSERT_EQ(str, ss.str());
    }
};

TEST_F(rask_Position, defaultPosition)
{
    Position p;

    ASSERT_EQ("", p.file);
    ASSERT_EQ(0u, p.row);
    ASSERT_EQ(0u, p.column);
}

TEST_F(rask_Position, constructFromFileRowColumn)
{
    Position p("asia", 7, 19);

    ASSERT_EQ("asia", p.file);
    ASSERT_EQ(7u, p.row);
    ASSERT_EQ(19u, p.column);
}

TEST_F(rask_Position, equality)
{
    ASSERT_TRUE(Position("abc", 123, 456) == Position("abc", 123, 456));
    ASSERT_FALSE(Position("abc", 5123, 274) == Position("abc", 123, 456));
    ASSERT_FALSE(Position("abc", 5123, 274) == Position("abc", 123, 274));
    ASSERT_FALSE(Position("abc", 5123, 274) == Position("abc", 5123, 456));
    ASSERT_FALSE(Position("abc", 123, 456) == Position("xxx", 123, 456));
}


TEST_F(rask_Position, inequality)
{
    ASSERT_FALSE(Position("abc", 123, 456) != Position("abc", 123, 456));
    ASSERT_TRUE(Position("abc", 5123, 274) != Position("abc", 123, 456));
    ASSERT_TRUE(Position("abc", 5123, 274) != Position("abc", 123, 274));
    ASSERT_TRUE(Position("abc", 5123, 274) != Position("abc", 5123, 456));
    ASSERT_TRUE(Position("abc", 123, 456) != Position("xxx", 123, 456));
}

TEST_F(rask_Position, less)
{
    ASSERT_FALSE(Position("abc", 123, 456) < Position("abc", 123, 456));
    ASSERT_FALSE(Position("abc", 123, 456) < Position("abc", 123, 450));
    ASSERT_FALSE(Position("abc", 123, 456) < Position("abc", 120, 450));
    ASSERT_FALSE(Position("zzz", 123, 456) < Position("abc", 123, 456));
    ASSERT_TRUE(Position("abc", 123, 456) < Position("abc", 123, 457));
    ASSERT_TRUE(Position("abc", 123, 456) < Position("abc", 124, 457));
    ASSERT_TRUE(Position("abc", 123, 456) < Position("zzz", 123, 456));
}

TEST_F(rask_Position, shiftToStream)
{
    assertString("abc:123:456", Position("abc", 123, 456));
    assertString("", Position());
    assertString("abc:100", Position("abc", 100, 0));
    assertString("abc:100", Position("abc", 100));
    assertString("abc", Position("abc", 0, 0));
    assertString("abc", Position("abc"));
}

TEST_F(rask_Position, constructFromFile)
{
    Position p("asia");

    ASSERT_EQ("asia", p.file);
    ASSERT_EQ(0u, p.row);
    ASSERT_EQ(0u, p.column);
}

TEST_F(rask_Position, constructFromFileRow)
{
    Position p("asia", 10);

    ASSERT_EQ("asia", p.file);
    ASSERT_EQ(10u, p.row);
    ASSERT_EQ(0u, p.column);
}

