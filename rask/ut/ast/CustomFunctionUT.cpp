// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/CustomFunction.hpp>
#include <boost/assign/list_of.hpp>
#include <rask/test/VariableDeclFactory.hpp>
#include <rask/test/VariableFactory.hpp>
#include <gtest/gtest.h>

using namespace rask;
using namespace testing;

struct rask_ast_CustomFunction : testing::Test
{
    test::VariableDeclFactory varDeclFactory;
};

TEST_F(rask_ast_CustomFunction, twoStatements)
{
    cst::Identifier name = cst::Identifier::create(Position("abc", 10, 20), "kasia");
    ast::CustomFunction cf(name, ast::VOID);
    ast::Function& f = cf;
    ast::SharedCustomFunction dummy(new ast::CustomFunction(cst::Identifier::create(Position(), "test"), ast::VOID));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());
    ast::VariableDecl vd = varDeclFactory.create("asia");
    ASSERT_EQ(0u, cf.stmtCount());
    cf.addStmt(fc);
    cf.addStmt(vd);
    ASSERT_EQ(name.position, f.name().position);
    ASSERT_EQ(name.value, f.name().value);
    ASSERT_EQ(0u, f.argCount());
    ASSERT_EQ(0u, f.argTypes().size());
    ASSERT_EQ(2u, cf.stmtCount());
    ASSERT_TRUE(getFunctionCall(cf.stmt(0)).function().lock() == dummy);
    ASSERT_EQ(0u, getFunctionCall(cf.stmt(0)).args().size());
    ASSERT_TRUE(getVariableDecl(cf.stmt(1)).var() == vd.var());
}

TEST_F(rask_ast_CustomFunction, arguments)
{
    using boost::assign::list_of;

    ast::CustomFunction cf(cst::Identifier::create(Position(), "xxx"), ast::VOID);
    test::VariableFactory variableFactory;
    ast::SharedVariable v1 = variableFactory.createShared("a", ast::INT32);
    ast::SharedVariable v2 = variableFactory.createShared("b", ast::BOOLEAN);

    cf.addArg(v1);
    ASSERT_TRUE(cf.argTypes() == list_of(ast::INT32));
    ASSERT_EQ(1u, cf.argCount());
    ASSERT_TRUE(cf.arg(0) ==  v1);
    cf.addArg(v2);
    ASSERT_TRUE(cf.argTypes() == list_of(ast::INT32)(ast::BOOLEAN));
    ASSERT_EQ(2u, cf.argCount());
    ASSERT_TRUE(cf.arg(1) ==  v2);
}

TEST_F(rask_ast_CustomFunction, type)
{
    ast::CustomFunction cf1(cst::Identifier::create(Position(), "abc"), ast::VOID);
    ast::CustomFunction cf2(cst::Identifier::create(Position(), "abc"), ast::INT32);

    ASSERT_TRUE(cf1.type() == ast::VOID);
    ASSERT_TRUE(cf2.type() == ast::INT32);
}

