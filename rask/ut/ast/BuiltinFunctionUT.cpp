// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/BuiltinFunction.hpp>
#include <gtest/gtest.h>

using namespace rask;

TEST(rask_ast_BuiltinFunction, constructor)
{
    ast::BuiltinFunction bf1("test1", ast::VOID, 2);
    ast::BuiltinFunction bf2("test2", ast::INT32, 0);
    ast::Function& f1 = bf1;
    ast::Function& f2 = bf2;

    ASSERT_TRUE(f1.name().position == Position());
    ASSERT_EQ("test1", f1.name().value);
    ASSERT_EQ(2u, f1.argCount());
    ASSERT_TRUE(f1.type() == ast::VOID);
    ASSERT_TRUE(f2.name().position == Position());
    ASSERT_EQ("test2", f2.name().value);
    ASSERT_EQ(0u, f2.argCount());
    ASSERT_TRUE(f2.type() == ast::INT32);
}
