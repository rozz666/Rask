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
    BuilderMock(error::Logger& logger, ast::FunctionTable& ft)
        : ast::Builder(logger, ft) { }

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
    error::Logger logger;
    ast::FunctionTable ft;
    BuilderMock builder;
    ast::SharedScopeMock scopeMock;
    ast::SharedScope scope;

    rask_ast_buildExpression() : builder(logger, ft), scopeMock(new ast::ScopeMock), scope(scopeMock) { }
};

TEST_F(rask_ast_buildExpression, constant)
{
    cst::Constant c = cst::Constant::create(Position(), 123);
    boost::optional<ast::Expression> expr = builder.buildExpression(c, null);

    ASSERT_TRUE(expr);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getConstant(*expr) == ast::Constant(c.value));
}

TEST_F(rask_ast_buildExpression, variable)
{
    cst::Identifier id = cst::Identifier::create(Position(), "abc");
    ast::SharedVariable var(test::VariableFactory().createShared(id));

    EXPECT_CALL(*scopeMock, getVariable(id.value))
        .WillOnce(Return(var));

    boost::optional<ast::Expression> expr = builder.buildExpression(cst::Expression(id), scope);

    ASSERT_TRUE(expr);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getVariable(*expr).lock() == var);
}

TEST_F(rask_ast_buildExpression, variableFails)
{
    EXPECT_CALL(*scopeMock, getVariable(_))
        .WillOnce(Return(null));

    cst::Identifier id = cst::Identifier::create(Position("xxx", 1, 2), "abc");

    ASSERT_FALSE(builder.buildExpression(id, scopeMock));
    ASSERT_EQ(1u, logger.errors().size());
    ASSERT_EQ(error::Message::unknownIdentifier(id.position, id.value), logger.errors()[0]);
}

TEST_F(rask_ast_buildExpression, functionCall)
{
    cst::Expression fc = cst::FunctionCall();
    unsigned n = 5;

    EXPECT_CALL(builder, buildFunctionCall(Ref(getFunctionCall(fc)), scope))
        .WillOnce(Return(ast::FunctionCall(null, ast::FunctionCall::Arguments(n))));

    boost::optional<ast::Expression> expr = builder.buildExpression(fc, scope);

    ASSERT_TRUE(expr);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getFunctionCall(*expr).args().size() == n);
}

TEST_F(rask_ast_buildExpression, functionCallFails)
{
    cst::Expression fc = cst::FunctionCall();

    EXPECT_CALL(builder, buildFunctionCall(_, _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildExpression(fc, null));
    ASSERT_TRUE(logger.errors().empty());
}

TEST_F(rask_ast_buildExpression, unaryOperatorCall)
{
    cst::Expression c = cst::UnaryOperatorCall();
    unsigned n = 5;

    EXPECT_CALL(builder, buildUnaryOperatorCall(Ref(getUnaryOperatorCall(c)), scope))
        .WillOnce(Return(ast::FunctionCall(null, ast::FunctionCall::Arguments(n))));

    boost::optional<ast::Expression> expr = builder.buildExpression(c, scopeMock);

    ASSERT_TRUE(expr);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getFunctionCall(*expr).args().size() == n);
}

TEST_F(rask_ast_buildExpression, unaryOperatorCallFails)
{
    cst::Expression c = cst::UnaryOperatorCall();

    EXPECT_CALL(builder, buildUnaryOperatorCall(_, _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildExpression(c, null));
    ASSERT_TRUE(logger.errors().empty());
}

TEST_F(rask_ast_buildExpression, chainExpression)
{
    ast::Constant ret(5);
    cst::Expression ce = cst::ChainExpression();

    EXPECT_CALL(builder, buildChainExpression(Ref(getChainExpression(ce)), Eq<ast::SharedScope>(null)))
        .WillOnce(Return(ast::Expression(ret)));

    boost::optional<ast::Expression> expr = builder.buildExpression(ce, null);

    ASSERT_TRUE(expr);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getConstant(*expr) == ret);
}

TEST_F(rask_ast_buildExpression, chainExpressionFails)
{
    cst::ChainExpression ce;

    EXPECT_CALL(builder, buildChainExpression(_, _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildExpression(ce, scopeMock));
    ASSERT_TRUE(logger.errors().empty());
}

TEST_F(rask_ast_buildExpression, booleanConstants)
{
    cst::Identifier c1 = cst::Identifier::create(Position(), "true");
    cst::Identifier c2 = cst::Identifier::create(Position(), "false");
    boost::optional<ast::Expression> expr1 = builder.buildExpression(c1, null);
    boost::optional<ast::Expression> expr2 = builder.buildExpression(c2, null);

    ASSERT_TRUE(expr1);
    ASSERT_TRUE(expr2);
    ASSERT_TRUE(logger.errors().empty());
    ASSERT_TRUE(getConstant(*expr1) == ast::Constant(true));
    ASSERT_TRUE(getConstant(*expr2) == ast::Constant(false));
}

