// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <rask/ast/Tree.hpp>
#include <rask/test/FunctionFactory.hpp>

using namespace rask;

struct rask_ast_Tree : ::testing::Test
{
    ast::Tree tree;
};

TEST_F(rask_ast_Tree, emptyTree)
{
    ASSERT_EQ(0u, tree.functionCount());
}

TEST_F(rask_ast_Tree, twoFunctions)
{
    ast::SharedCustomFunction f1 = test::FunctionFactory::createShared("abc");
    ast::SharedCustomFunction f2 = test::FunctionFactory::createShared("def");

    tree.add(f1);
    ASSERT_TRUE(tree.function(0) == f1);
    ASSERT_EQ(1u, tree.functionCount());

    tree.add(f2);
    ASSERT_TRUE(tree.function(1) == f2);
    ASSERT_EQ(2u, tree.functionCount());
}

