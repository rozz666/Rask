// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <rask/ast/Scope.hpp>
#include <rask/test/VariableFactory.hpp>

struct rask_ast_Scope : testing::Test
{
    rask::test::VariableFactory variableFactory;
    std::string var1Name;
    std::string var2Name;
    rask::ast::SharedVariable var1a;
    rask::ast::SharedVariable var1b;
    rask::ast::SharedVariable var2;
    rask::ast::Scope scope;

    rask_ast_Scope()
        : var1Name("asia"), var2Name("kasia"),
        var1a(variableFactory.createShared(var1Name)),
        var1b(variableFactory.createShared(var1Name)),
        var2(variableFactory.createShared(var2Name)) { }

};

TEST_F(rask_ast_Scope, addTwoVariables)
{
    ASSERT_TRUE(scope.addVariable(var1a) == var1a);
    ASSERT_TRUE(scope.addVariable(var2) == var2);
    ASSERT_TRUE(*scope.getVariable(var1Name) == var1a);
    ASSERT_TRUE(*scope.getVariable(var2Name) == var2);
}

TEST_F(rask_ast_Scope, getBadVariable)
{
    ASSERT_TRUE(!scope.getVariable("x"));
}

TEST_F(rask_ast_Scope, addTwoVariablesWithSameName)
{
    ASSERT_TRUE(scope.addVariable(var1a) == var1a);
    ASSERT_TRUE(scope.addVariable(var1b) == var1a);
    ASSERT_TRUE(*scope.getVariable(var1Name) == var1a);
}
