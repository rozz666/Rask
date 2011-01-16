// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/BuiltinFunctions.hpp>
#include <rask/ast/FunctionTable.hpp>
#include <rask/Operators.hpp>
#include <gtest/gtest.h>

struct rask_ast_BuiltinFunctions : testing::Test
{
    rask::ast::BuiltinFunctions bfs;
    rask::ast::FunctionTable ft;

    rask_ast_BuiltinFunctions()
    {
        bfs.declare(ft);
    }

    void assertBuiltinFunction(const std::string& name, rask::ast::BasicType type, unsigned argCount)
    {
        using namespace rask;

        boost::optional<ast::SharedFunction> f = ft.getFunction(name);
        ASSERT_TRUE(f);
        ASSERT_TRUE(boost::dynamic_pointer_cast<ast::BuiltinFunction>(*f));
        ASSERT_EQ(Position(), (*f)->name().position);
        ASSERT_EQ(name, (*f)->name().value);
        ASSERT_EQ(argCount, (*f)->argCount());
        ASSERT_TRUE((*f)->type() == type);
    }
};

TEST_F(rask_ast_BuiltinFunctions, print)
{
    assertBuiltinFunction("print", rask::ast::VOID, 1);
}

TEST_F(rask_ast_BuiltinFunctions, getInt32)
{
    assertBuiltinFunction("getInt32", rask::ast::INT32, 0);;
}

TEST_F(rask_ast_BuiltinFunctions, unaryMinus)
{
    assertBuiltinFunction(rask::UNARY_MINUS_NAME, rask::ast::INT32, 1);
}

TEST_F(rask_ast_BuiltinFunctions, binaryMinus)
{
    assertBuiltinFunction(rask::BINARY_MINUS_NAME, rask::ast::INT32, 2);
}

TEST_F(rask_ast_BuiltinFunctions, binaryPlus)
{
    assertBuiltinFunction(rask::BINARY_PLUS_NAME, rask::ast::INT32, 2);
}

TEST_F(rask_ast_BuiltinFunctions, binaryMult)
{
    assertBuiltinFunction(rask::BINARY_MULT_NAME, rask::ast::INT32, 2);
}

TEST_F(rask_ast_BuiltinFunctions, binaryDiv)
{
    assertBuiltinFunction(rask::BINARY_DIV_NAME, rask::ast::INT32, 2);
}

TEST_F(rask_ast_BuiltinFunctions, binaryMod)
{
    assertBuiltinFunction(rask::BINARY_MOD_NAME, rask::ast::INT32, 2);
}
