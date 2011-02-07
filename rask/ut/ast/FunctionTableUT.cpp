// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/FunctionTable.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <boost/assign/list_of.hpp>
#include <gtest/gtest.h>

using namespace rask;
using namespace testing;

struct rask_ast_FunctionTable : testing::Test
{
    ast::FunctionTable ft;
    std::string f1Name;
    std::string f2Name;
    ast::SharedFunction f1a;
    ast::SharedFunction f1b;
    ast::SharedFunction f2;

    rask_ast_FunctionTable()
        : f1Name("asia"), f2Name("kasia"),
        f1a(test::FunctionFactory::createShared(f1Name)),
        f1b(test::FunctionFactory::createShared(f1Name)),
        f2(test::FunctionFactory::createShared(f2Name))
    {
    }
};

TEST_F(rask_ast_FunctionTable, twoFunctions)
{
    ASSERT_TRUE(ft.add(f1a) == f1a);
    ASSERT_TRUE(ft.add(f2) == f2);
    ASSERT_TRUE(*ft.getFunction(f1Name) == f1a);
    ASSERT_TRUE(*ft.getFunction(f2Name) == f2);
}

TEST_F(rask_ast_FunctionTable, badFunctionName)
{
    ASSERT_FALSE(ft.getFunction("x"));
}

TEST_F(rask_ast_FunctionTable, duplicatedFunction)
{
    ASSERT_TRUE(ft.add(f1a) == f1a);
    ASSERT_TRUE(ft.add(f1b) == f1a);
    ASSERT_TRUE(ft.getFunction(f1Name) == f1a);
}

TEST_F(rask_ast_FunctionTable, family)
{
    using boost::assign::list_of;

    std::string name("fam1");
    ast::Function::ArgumentTypes args1 = list_of(ast::INT32)(ast::INT32);
    ast::SharedCustomFunction fam11 = test::FunctionFactory::createShared(name, ast::VOID, args1);
    ast::Function::ArgumentTypes args2 = list_of(ast::BOOLEAN)(ast::INT32);
    ast::SharedCustomFunction fam12 = test::FunctionFactory::createShared(name, ast::VOID, args2);
    ast::Function::ArgumentTypes args3 = list_of(ast::INT32)(ast::BOOLEAN);
    ast::SharedCustomFunction fam13 = test::FunctionFactory::createShared(name, ast::VOID, args3);

    ASSERT_TRUE(ft.add(fam11) == fam11);
    ASSERT_TRUE(ft.add(fam12) == fam12);
    ASSERT_TRUE(ft.add(fam13) == fam13);
    boost::optional<ast::SharedFunctionFamily> optFam1 = ft.getFamily(name);
    ASSERT_TRUE(optFam1);
    ast::SharedFunctionFamily fam1 = *optFam1;
    ASSERT_EQ(name, fam1->name());
    ASSERT_TRUE(fam1->getFunction(args1));
    ASSERT_TRUE(*fam1->getFunction(args1) == fam11);
    ASSERT_TRUE(fam1->getFunction(args2));
    ASSERT_TRUE(*fam1->getFunction(args2) == fam12);
    ASSERT_TRUE(fam1->getFunction(args3));
    ASSERT_TRUE(*fam1->getFunction(args3) == fam13);
}

TEST_F(rask_ast_FunctionTable, invalidFamily)
{
    ASSERT_FALSE(ft.getFamily("xxx"));
}
