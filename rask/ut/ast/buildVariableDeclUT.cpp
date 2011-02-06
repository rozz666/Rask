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
#include <rask/ut/ast/ScopeMockNew.hpp>
#include <rask/ut/ast/VariableFactoryMockNew.hpp>
#include <rask/ut/cst/OperatorStubs.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(
        error::SharedLogger logger, ast::SharedFunctionTable ft, ast::SharedVariableFactory variableFactory)
        : ast::Builder(logger, ft, variableFactory) { }

    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

}

struct rask_ast_Builder_buildVariableDecl : testing::Test
{
    cst::VariableDecl cvd;
    error::SharedLogger logger;
    ast::SharedScopeMock scope;
    ast::SharedVariableFactoryMock variableFactory;
    BuilderMock builder;

    rask_ast_Builder_buildVariableDecl()
        : logger(new error::Logger()), scope(new ast::ScopeMock),
        variableFactory(new ast::VariableFactoryMock), builder(logger, null, variableFactory)
    {
        cvd.value = cst::ChainExpression();
    }
};

TEST_F(rask_ast_Builder_buildVariableDecl, successful)
{
    ast::Constant dummy(123);
    ast::SharedVariable v = test::VariableFactory().createShared();
    EXPECT_CALL(builder, buildExpression(Ref(*cvd.value), Eq<ast::SharedScope>(scope)))
        .WillOnce(Return(ast::Expression(dummy)));
    EXPECT_CALL(*variableFactory, createVariable(Ref(cvd.name), dummy.type()))
        .WillOnce(Return(v));
    EXPECT_CALL(*scope, addVariable(v))
        .WillOnce(Return(v));

    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd, scope);

    ASSERT_TRUE(vd);
    ASSERT_TRUE(logger->errors().empty());
    ASSERT_TRUE(vd->var() == v);
    ASSERT_TRUE(getConstant(vd->value()) == dummy);
}

TEST_F(rask_ast_Builder_buildVariableDecl, noValue)
{
    cvd.name = cst::Identifier::create(Position("abc", 1, 3), "x");
    cvd.value = boost::none;

    ASSERT_FALSE(builder.buildVariableDecl(cvd, scope));
    ASSERT_EQ(1u, logger->errors().size());
    ASSERT_EQ(error::Message::uninitializedVariable(cvd.name.position, cvd.name.value), logger->errors()[0]);
}

TEST_F(rask_ast_Builder_buildVariableDecl, badExpression)
{
    EXPECT_CALL(builder, buildExpression(_, _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildVariableDecl(cvd, scope));
    ASSERT_TRUE(logger->errors().empty());
}
