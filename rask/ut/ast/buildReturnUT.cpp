// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/ut/ast/ScopeMockNew.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock() : ast::Builder(null, null, null) { }

    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

std::ostream& operator<<(std::ostream& os, const cst::Expression& )
{
    return os << "cst::Expression";
}

}

struct rask_ast_Builder_buildReturn : testing::Test
{
    cst::Return ret;
    ast::SharedScope scope;
    BuilderMock builder;

    rask_ast_Builder_buildReturn() : scope(new ast::ScopeMock) { }
};

TEST_F(rask_ast_Builder_buildReturn, successful)
{
    ast::Constant value(555);
    EXPECT_CALL(builder, buildExpression(Ref(ret.value), scope))
        .WillOnce(Return(ast::Expression(value)));

    boost::optional<ast::Return> r = builder.buildReturn(ret, scope);
    ASSERT_TRUE(r);
    ASSERT_TRUE(getConstant(r->expr()) == value);
}

TEST_F(rask_ast_Builder_buildReturn, buildExpressionFails)
{
    EXPECT_CALL(builder, buildExpression(_, _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildReturn(ret, scope));
}
