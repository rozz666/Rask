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
#include <rask/ut/cst/OperatorStubs.hpp>
#include <rask/null.hpp>
#include <gtest/gtest.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(ast::SharedFunctionTable ft) : ast::Builder(null, ft, null) { }

    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

}

struct rask_ast_Builder_buildUnaryOperatorCall : testing::Test
{
    ast::SharedFunctionTable ft;
    BuilderMock builder;
    ast::SharedScope scope;

    rask_ast_Builder_buildUnaryOperatorCall()
        : ft(new ast::FunctionTable), builder(ft), scope(new ast::ScopeMock) { }
};

TEST_F(rask_ast_Builder_buildUnaryOperatorCall, minus)
{
    cst::UnaryOperatorCall oc;
    oc.op.tag = cst::UnaryOperator::MINUS;

    ast::SharedCustomFunction f = test::FunctionFactory().createShared(UNARY_MINUS_NAME, ast::INT32, 1);
    ast::Constant retExpr(7);
    EXPECT_CALL(builder, buildExpression(Ref(oc.expr), scope))
        .WillOnce(Return(ast::Expression(retExpr)));

    ft->add(f);

    boost::optional<ast::FunctionCall> fc = builder.buildUnaryOperatorCall(oc, scope);

    ASSERT_TRUE(fc);
    ASSERT_TRUE(fc->function().lock() == f);
    ASSERT_EQ(1u, fc->args().size());
    ASSERT_TRUE(getConstant(fc->args()[0]) == retExpr);
}

