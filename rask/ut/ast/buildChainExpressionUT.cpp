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
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(error::Logger& logger, ast::FunctionTable& ft)
        : ast::Builder(logger, ft) { }

    MOCK_METHOD1(buildFunctionCall, boost::optional<ast::FunctionCall>(const cst::FunctionCall& ));
    MOCK_METHOD1(buildUnaryOperatorCall, boost::optional<ast::FunctionCall>(const cst::UnaryOperatorCall&));
    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

std::ostream& operator<<(std::ostream& os, const cst::Expression& )
{
    return os << "cst::Expression";
}

}

struct rask_ast_Builder_buildChainExpression : testing::Test
{
    error::Logger logger;
    ast::FunctionTable ft;
    BuilderMock builder;
    ast::SharedScope scope;
    cst::ChainExpression e;
    boost::optional<ast::Expression> expr;

    rask_ast_Builder_buildChainExpression() : builder(logger, ft), scope(new ast::ScopeMock) { }

    void assertBuildChainExpression()
    {
        expr = builder.buildChainExpression(e, scope);
        ASSERT_TRUE(expr);
        ASSERT_TRUE(logger.errors().empty());
    }

    void assertFailedBuildChainExpression()
    {
        ASSERT_FALSE(builder.buildChainExpression(e, scope));
        ASSERT_TRUE(logger.errors().empty());
    }
};

TEST_F(rask_ast_Builder_buildChainExpression, expression)
{
    ast::Constant c(7);

    EXPECT_CALL(builder, buildExpression(Ref(e.expr), scope))
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
    e.next.resize(2);
    e.next[0].op.tag = cst::BinaryOperator::MINUS;
    e.next[1].op.tag = cst::BinaryOperator::PLUS;

    EXPECT_CALL(builder, buildExpression(Ref(e.expr), _))
        .WillOnce(Return(ast::Expression(a)));
    EXPECT_CALL(builder, buildExpression(Ref(e.next[0].expr), _))
        .WillOnce(Return(ast::Expression(b)));
    EXPECT_CALL(builder, buildExpression(Ref(e.next[1].expr), _))
        .WillOnce(Return(ast::Expression(c)));

    test::FunctionFactory functionFactory;
    ast::SharedCustomFunction opMinus = functionFactory.createShared(BINARY_MINUS_NAME, ast::INT32, 2);
    ast::SharedCustomFunction opPlus = functionFactory.createShared(BINARY_PLUS_NAME, ast::INT32, 2);
    ft.add(opMinus);
    ft.add(opPlus);

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
    e.next.resize(1);

    EXPECT_CALL(builder, buildExpression(_, _))
        .WillOnce(Return(ast::Expression()));
    EXPECT_CALL(builder, buildExpression(Ref(e.next[0].expr), _))
        .WillOnce(Return(boost::none));
    assertFailedBuildChainExpression();
}
