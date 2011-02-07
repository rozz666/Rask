// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <rask/Operators.hpp>
#include <rask/ut/ast/ScopeMockNew.hpp>
#include <rask/null.hpp>
#include <rask/ut/cst/OperatorStubs.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(ast::SharedFunctionTable ft)
        : ast::Builder(null, ft, null) { }

    MOCK_METHOD1(buildFunctionCall, boost::optional<ast::FunctionCall>(const cst::FunctionCall& ));
    MOCK_METHOD1(buildUnaryOperatorCall, boost::optional<ast::FunctionCall>(const cst::UnaryOperatorCall&));
    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

}

struct rask_ast_Builder_buildChainExpression : testing::Test
{
    ast::SharedFunctionTable ft;
    BuilderMock builder;
    ast::SharedScope scope;
    cst::ChainExpression chainExpr;
    boost::optional<ast::Expression> expr;

    rask_ast_Builder_buildChainExpression()
        : ft(new ast::FunctionTable), builder(ft), scope(new ast::ScopeMock) { }

    void assertBuildChainExpression()
    {
        expr = builder.buildChainExpression(chainExpr, scope);
        ASSERT_TRUE(expr);
    }

    void assertFailedBuildChainExpression()
    {
        ASSERT_FALSE(builder.buildChainExpression(chainExpr, scope));
    }
};

TEST_F(rask_ast_Builder_buildChainExpression, expression)
{
    ast::Constant c(7);

    EXPECT_CALL(builder, buildExpression(Ref(chainExpr.expr), scope))
        .WillOnce(Return(ast::Expression(c)));

    ASSERT_NO_FATAL_FAILURE(assertBuildChainExpression());
    ASSERT_TRUE(getConstant(*expr) == c);
}

TEST_F(rask_ast_Builder_buildChainExpression, buildExpressionFails)
{
    EXPECT_CALL(builder, buildExpression(_, _))
        .WillOnce(Return(boost::none));
    assertFailedBuildChainExpression();
}

TEST_F(rask_ast_Builder_buildChainExpression, operators)
{
    ast::Constant a(1), b(2), c(3);
    chainExpr.next.resize(2);
    chainExpr.next[0].op.tag = cst::BinaryOperator::MINUS;
    chainExpr.next[1].op.tag = cst::BinaryOperator::PLUS;

    EXPECT_CALL(builder, buildExpression(Ref(chainExpr.expr), _))
        .WillOnce(Return(ast::Expression(a)));
    EXPECT_CALL(builder, buildExpression(Ref(chainExpr.next[0].expr), _))
        .WillOnce(Return(ast::Expression(b)));
    EXPECT_CALL(builder, buildExpression(Ref(chainExpr.next[1].expr), _))
        .WillOnce(Return(ast::Expression(c)));

    ast::SharedCustomFunction opMinus = test::FunctionFactory::createShared(BINARY_MINUS_NAME, ast::INT32, 2);
    ast::SharedCustomFunction opPlus = test::FunctionFactory::createShared(BINARY_PLUS_NAME, ast::INT32, 2);
    ft->add(opMinus);
    ft->add(opPlus);

    ASSERT_NO_FATAL_FAILURE(assertBuildChainExpression());

    ast::FunctionCall& fc1 = getFunctionCall(*expr);
    ASSERT_TRUE(fc1.function().lock() == opPlus);
    ASSERT_EQ(2u, fc1.args().size());
    const ast::FunctionCall& fc2 = getFunctionCall(fc1.args()[0]);
    ASSERT_TRUE(fc2.function().lock() == opMinus);
    ASSERT_EQ(2u, fc2.args().size());
    ASSERT_TRUE(getConstant(fc2.args()[0]) == a);
    ASSERT_TRUE(getConstant(fc2.args()[1]) == b);
    ASSERT_TRUE(getConstant(fc1.args()[1]) == c);
}

TEST_F(rask_ast_Builder_buildChainExpression, buildExpressionSecondFails)
{
    chainExpr.next.resize(1);

    EXPECT_CALL(builder, buildExpression(_, _))
        .WillOnce(Return(ast::Expression()));
    EXPECT_CALL(builder, buildExpression(Ref(chainExpr.next[0].expr), _))
        .WillOnce(Return(boost::none));
    assertFailedBuildChainExpression();
}
