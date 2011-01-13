// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Constant.hpp>
#include <gtest/gtest.h>

using namespace rask::ast;

TEST(rask_ast_Constant, int32Equality)
{
    boost::int32_t n = 35;
    Constant c1(boost::int32_t(77));
    Constant c2(n);
    Constant c3(n);

    ASSERT_TRUE(c1 != c2);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c2 == c3);
    ASSERT_FALSE(c2 != c3);
}

TEST(rask_ast_Constant, getInt32)
{
    boost::int32_t n1 = 77;
    boost::int32_t n2 = 35;
    Constant c1(n1);
    Constant c2(n2);

    ASSERT_EQ(n1, c1.getInt32());
    ASSERT_EQ(n2, c2.getInt32());
}

TEST(rask_ast_Constant, booleanEquality)
{
    Constant c1(true);
    Constant c2(false);
    Constant c3(false);

    ASSERT_TRUE(c1 != c2);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c2 == c3);
    ASSERT_FALSE(c2 != c3);
}

TEST(rask_ast_Constant, getBoolean)
{
    bool v1 = true;
    bool v2 = false;
    Constant c1(v1);
    Constant c2(v2);

    ASSERT_EQ(v1, c1.getBoolean());
    ASSERT_EQ(v2, c2.getBoolean());
}

TEST(rask_ast_Constant, mixedEquality)
{
    ASSERT_TRUE(Constant(false) != Constant(boost::int32_t(0)));
    ASSERT_FALSE(Constant(false) == Constant(boost::int32_t(0)));
    ASSERT_TRUE(Constant(boost::int32_t(0)) != Constant(false));
    ASSERT_FALSE(Constant(boost::int32_t(0)) == Constant(false));
}

TEST(rask_ast_Constant, typeQuery)
{
    ASSERT_TRUE(Constant(boost::int32_t(11)).type() == INT32);
    ASSERT_TRUE(Constant(true).type() == BOOLEAN);
}

