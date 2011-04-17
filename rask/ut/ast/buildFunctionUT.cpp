// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/test/VariableDeclFactory.hpp>
#include <rask/test/VariableFactory.hpp>
#include <rask/null.hpp>
#include <rask/ut/ast/ScopeMock.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock() : ast::Builder(null, null, null) { }

    MOCK_METHOD2(buildVariableDecl, boost::optional<ast::VariableDecl>(const cst::VariableDecl&, ast::SharedScope));
    MOCK_METHOD2(buildFunctionCall, boost::optional<ast::FunctionCall>(const cst::FunctionCall&, ast::SharedScope));
    MOCK_METHOD2(buildReturn, boost::optional<ast::Return>(const cst::Return&, ast::SharedScope));
};

}

struct rask_ast_Builder_buildFunction : testing::Test
{
    const std::string file;
    cst::Function cf;
    ast::SharedCustomFunction f;
    ast::test::SharedScopeMock scopeMock;
    ast::SharedScope scope;
    BuilderMock builder;

    rask_ast_Builder_buildFunction()
        : file("test.rask"), scopeMock(new ast::test::ScopeMock), scope(scopeMock)
    {
        cf.name = cst::Identifier::create(Position(file, 1, 2), "main");
        cf.type = cst::Identifier::create(Position(file, 1, 10), "void");

        f.reset(new ast::CustomFunction(cf.name, ast::VOID));
    }
};

TEST_F(rask_ast_Builder_buildFunction, 1)
{
    ASSERT_TRUE(builder.buildFunction(cf, f, scope));
    ASSERT_EQ(0u, f->stmtCount());
}


TEST_F(rask_ast_Builder_buildFunction, 2)
{
    const unsigned n1 = 1;
    const unsigned n2 = 2;

    cf.stmts.resize(2, cst::FunctionCall());

    {
        InSequence seq;
        EXPECT_CALL(builder, buildFunctionCall(Ref(getFunctionCall(cf.stmts[0])), scope))
            .WillOnce(Return(ast::FunctionCall(null, ast::FunctionCall::Arguments(n1))));
        EXPECT_CALL(builder, buildFunctionCall(Ref(getFunctionCall(cf.stmts[1])), scope))
            .WillOnce(Return(ast::FunctionCall(null, ast::FunctionCall::Arguments(n2))));
    }

    ASSERT_TRUE(builder.buildFunction(cf, f, scope));

    ASSERT_EQ(2u, f->stmtCount());
    ASSERT_EQ(n1, getFunctionCall(f->stmt(0)).args().size());
    ASSERT_EQ(n2, getFunctionCall(f->stmt(1)).args().size());
}

TEST_F(rask_ast_Builder_buildFunction, 3)
{
    cf.stmts.resize(2, cst::FunctionCall());

    EXPECT_CALL(builder, buildFunctionCall(Ref(getFunctionCall(cf.stmts[0])), scope))
        .WillOnce(Return(boost::none));

    ASSERT_FALSE(builder.buildFunction(cf, f, scope));
}

TEST_F(rask_ast_Builder_buildFunction, 4)
{
    const std::string n1 = "a";
    const std::string n2 = "b";

    cf.stmts.resize(2, cst::VariableDecl());
    {
        InSequence seq;
        EXPECT_CALL(builder, buildVariableDecl(Ref(getVariableDecl(cf.stmts[0])), scope))
            .WillOnce(Return(test::VariableDeclFactory::create(n1)));
        EXPECT_CALL(builder, buildVariableDecl(Ref(getVariableDecl(cf.stmts[1])), scope))
            .WillOnce(Return(test::VariableDeclFactory::create(n2)));
    }

    ASSERT_TRUE(builder.buildFunction(cf, f, scope));

    ASSERT_EQ(2u, f->stmtCount());
    ASSERT_EQ(n1, getVariableDecl(f->stmt(0)).var()->name().value);
    ASSERT_EQ(n2, getVariableDecl(f->stmt(1)).var()->name().value);
}

TEST_F(rask_ast_Builder_buildFunction, 5)
{
    cf.stmts.resize(2, cst::VariableDecl());

    EXPECT_CALL(builder, buildVariableDecl(Ref(getVariableDecl(cf.stmts[0])), scope))
        .WillOnce(Return(boost::none));

    ASSERT_FALSE(builder.buildFunction(cf, f, scope));
}

TEST_F(rask_ast_Builder_buildFunction, 6)
{
    ast::Constant c1(1);
    ast::Constant c2(2);

    cf.stmts.resize(2, cst::Return());

    {
        InSequence seq;
        EXPECT_CALL(builder, buildReturn(Ref(getReturn(cf.stmts[0])), scope))
            .WillOnce(Return(ast::Return(c1)));
        EXPECT_CALL(builder, buildReturn(Ref(getReturn(cf.stmts[1])), scope))
            .WillOnce(Return(ast::Return(c2)));
    }

    ASSERT_TRUE(builder.buildFunction(cf, f, scope));

    ASSERT_EQ(2u, f->stmtCount());
    ASSERT_TRUE(getConstant(getReturn(f->stmt(0)).expr()) == c1);
    ASSERT_TRUE(getConstant(getReturn(f->stmt(1)).expr()) == c2);
}

TEST_F(rask_ast_Builder_buildFunction, 7)
{
    cf.stmts.resize(2, cst::Return());

    EXPECT_CALL(builder, buildReturn(Ref(getReturn(cf.stmts[0])), scope))
        .WillOnce(Return(boost::none));

    ASSERT_FALSE(builder.buildFunction(cf, f, scope));
}

TEST_F(rask_ast_Builder_buildFunction, 8)
{
    ast::SharedVariable v1 = test::VariableFactory::createShared();
    ast::SharedVariable v2 = test::VariableFactory::createShared();

    f->addArg(v1);
    f->addArg(v2);

    EXPECT_CALL(*scopeMock, addVariable(v1))
        .WillOnce(Return(v1));
    EXPECT_CALL(*scopeMock, addVariable(v2))
        .WillOnce(Return(v2));

    ASSERT_TRUE(builder.buildFunction(cf, f, scope));
}
