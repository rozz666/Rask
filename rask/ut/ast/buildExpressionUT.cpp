// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/test/VariableFactory.hpp>
#include <rask/null.hpp>
#include <rask/ut/ast/ScopeMockNew.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(error::SharedLogger logger)
        : ast::Builder(logger, null, null) { }

    MOCK_METHOD2(
        buildFunctionCall, boost::optional<ast::FunctionCall>(const cst::FunctionCall&, ast::SharedScope));
    MOCK_METHOD2(
        buildUnaryOperatorCall, boost::optional<ast::FunctionCall>(const cst::UnaryOperatorCall&, ast::SharedScope));
    MOCK_METHOD2(
        buildChainExpression, boost::optional<ast::Expression>(const cst::ChainExpression&, ast::SharedScope));
};

}

struct rask_ast_buildExpression : testing::Test
{
    error::SharedLogger logger;
    BuilderMock builder;
    ast::SharedScopeMock scopeMock;
    ast::SharedScope scope;
    boost::optional<ast::Expression> expr;
    cst::Expression chainExpr;
    cst::Expression unOpCall;
    cst::Expression functionCall;
    ast::FunctionCall::Arguments fcArgs;

    rask_ast_buildExpression()
        : logger(new error::Logger), builder(logger), scopeMock(new ast::ScopeMock), scope(scopeMock), fcArgs(5)
    {
        chainExpr = cst::ChainExpression();
        unOpCall = cst::UnaryOperatorCall();
        functionCall = cst::FunctionCall();
    }

    void assertBuildExpression(const cst::Expression& cexpr, ast::SharedScope scope)
    {
        expr = builder.buildExpression(cexpr, scope);
        ASSERT_TRUE(expr);
        ASSERT_TRUE(logger->errors().empty());
    }

    void assertBuildExpressionWithScope(const cst::Expression& cexpr)
    {
        assertBuildExpression(cexpr, scope);
    }

    void assertBuildExpressionWithoutScope(const cst::Expression& cexpr)
    {
        assertBuildExpression(cexpr, null);
    }

    void assertBuildExpressionFailsNoErrors(const cst::Expression& cexpr)
    {
        ASSERT_FALSE(builder.buildExpression(cexpr, null));
        ASSERT_TRUE(logger->errors().empty());
    }
};

TEST_F(rask_ast_buildExpression, constant)
{
    cst::Constant c = cst::Constant::create(Position(), 123);
    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionWithoutScope(c));
    ASSERT_TRUE(getConstant(*expr) == ast::Constant(c.value));
}

TEST_F(rask_ast_buildExpression, variable)
{
    cst::Identifier id = cst::Identifier::create(Position(), "abc");
    ast::SharedVariable var(test::VariableFactory().createShared(id));

    EXPECT_CALL(*scopeMock, getVariable(id.value))
        .WillOnce(Return(var));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionWithScope(id));
    ASSERT_TRUE(getVariable(*expr).lock() == var);
}

TEST_F(rask_ast_buildExpression, variableFails)
{
    EXPECT_CALL(*scopeMock, getVariable(_))
        .WillOnce(Return(null));

    cst::Identifier id = cst::Identifier::create(Position("xxx", 1, 2), "abc");

    ASSERT_FALSE(builder.buildExpression(id, scope));
    ASSERT_EQ(1u, logger->errors().size());
    ASSERT_EQ(error::Message::unknownIdentifier(id.position, id.value), logger->errors()[0]);
}

TEST_F(rask_ast_buildExpression, functionCall)
{
    EXPECT_CALL(builder, buildFunctionCall(Ref(getFunctionCall(functionCall)), scope))
        .WillOnce(Return(ast::FunctionCall(null, fcArgs)));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionWithScope(functionCall));
    ASSERT_TRUE(getFunctionCall(*expr).args().size() == fcArgs.size());
}

TEST_F(rask_ast_buildExpression, functionCallFails)
{
    EXPECT_CALL(builder, buildFunctionCall(_, _))
        .WillOnce(Return(null));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionFailsNoErrors(functionCall));
}

TEST_F(rask_ast_buildExpression, unaryOperatorCall)
{
    EXPECT_CALL(builder, buildUnaryOperatorCall(Ref(getUnaryOperatorCall(unOpCall)), scope))
        .WillOnce(Return(ast::FunctionCall(null, fcArgs)));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionWithScope(unOpCall));
    ASSERT_TRUE(getFunctionCall(*expr).args().size() == fcArgs.size());
}

TEST_F(rask_ast_buildExpression, unaryOperatorCallFails)
{
    EXPECT_CALL(builder, buildUnaryOperatorCall(_, _))
        .WillOnce(Return(null));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionFailsNoErrors(unOpCall));
}

TEST_F(rask_ast_buildExpression, chainExpression)
{
    ast::Constant ret(5);
    EXPECT_CALL(builder, buildChainExpression(Ref(getChainExpression(chainExpr)), scope))
        .WillOnce(Return(ast::Expression(ret)));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionWithScope(chainExpr));
    ASSERT_TRUE(getConstant(*expr) == ret);
}

TEST_F(rask_ast_buildExpression, chainExpressionFails)
{
    EXPECT_CALL(builder, buildChainExpression(_, _))
        .WillOnce(Return(null));

    ASSERT_NO_FATAL_FAILURE(assertBuildExpressionFailsNoErrors(chainExpr));
}

TEST_F(rask_ast_buildExpression, booleanConstants)
{
    cst::Identifier c1 = cst::Identifier::create(Position(), "true");
    cst::Identifier c2 = cst::Identifier::create(Position(), "false");
    boost::optional<ast::Expression> expr1 = builder.buildExpression(c1, null);
    boost::optional<ast::Expression> expr2 = builder.buildExpression(c2, null);

    ASSERT_TRUE(expr1);
    ASSERT_TRUE(expr2);
    ASSERT_TRUE(logger->errors().empty());
    ASSERT_TRUE(getConstant(*expr1) == ast::Constant(true));
    ASSERT_TRUE(getConstant(*expr2) == ast::Constant(false));
}

