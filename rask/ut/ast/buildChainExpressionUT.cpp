// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <rask/Operators.hpp>
#include <rask/ut/ast/ScopeMock.hpp>

namespace
{

MOCK(BuilderMock, rask::ast::Builder)
{
    BuilderMock(rask::error::Logger& logger, rask::ast::FunctionTable& ft)
        : rask::ast::Builder(logger, ft) { }

    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildFunctionCall, (const rask::cst::FunctionCall&, fc))
    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildUnaryOperatorCall, (const rask::cst::UnaryOperatorCall&, oc))
    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildExpression,
        (const rask::cst::Expression&, expr)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildChainExpression_TestData
{
    rask::error::Logger logger;
    rask::ast::FunctionTable ft;
    BuilderMock builder;
    rask::ast::test::SharedScopeMock scope;

    buildChainExpression_TestData() : builder(logger, ft), scope(new rask::ast::test::ScopeMock) { }
};

typedef test_group<buildChainExpression_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildChainExpression");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::ChainExpression e;
    ast::Constant c(7);

    MOCK_RETURN(builder, buildExpression, ast::Expression(c));
    boost::optional<ast::Expression> expr = builder.buildChainExpression(e, scope);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getConstant(*expr) == c);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::ChainExpression e;

    MOCK_RETURN(builder, buildExpression, boost::none);
    ENSURE(!builder.buildChainExpression(e, scope));
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildExpression(e.expr, scope));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ast::Constant a(1), b(2), c(3);
    cst::ChainExpression e;
    e.next.resize(2);
    e.next[0].op.tag = cst::BinaryOperator::MINUS;
    e.next[1].op.tag = cst::BinaryOperator::PLUS;

    MOCK_RETURN(builder, buildExpression, ast::Expression(a));
    MOCK_RETURN(builder, buildExpression, ast::Expression(b));
    MOCK_RETURN(builder, buildExpression, ast::Expression(c));

    test::FunctionFactory functionFactory;
    ast::SharedCustomFunction opMinus = functionFactory.createShared(BINARY_MINUS_NAME, ast::INT32, 2);
    ast::SharedCustomFunction opPlus = functionFactory.createShared(BINARY_PLUS_NAME, ast::INT32, 2);
    ft.add(opMinus);
    ft.add(opPlus);

    boost::optional<ast::Expression> expr = builder.buildChainExpression(e, scope);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ast::FunctionCall& fc1 = getFunctionCall(*expr);
    ENSURE(fc1.function().lock() == opPlus);
    ENSURE_EQUALS(fc1.args().size(), 2u);
    const ast::FunctionCall& fc2 = getFunctionCall(fc1.args()[0]);
    ENSURE(fc2.function().lock() == opMinus);
    ENSURE_EQUALS(fc2.args().size(), 2u);
    ENSURE(getConstant(fc2.args()[0]) == a);
    ENSURE(getConstant(fc2.args()[1]) == b);
    ENSURE(getConstant(fc1.args()[1]) == c);
    ENSURE_CALL(builder, buildExpression(e.expr, scope));
    ENSURE_CALL(builder, buildExpression(e.next[0].expr, scope));
    ENSURE_CALL(builder, buildExpression(e.next[1].expr, scope));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    cst::ChainExpression e;
    e.next.resize(2);

    MOCK_RETURN(builder, buildExpression, ast::Expression(ast::Constant(1)));
    MOCK_RETURN(builder, buildExpression, boost::none);
    ENSURE(!builder.buildChainExpression(e, scope));
    ENSURE(logger.errors().empty());
}

}
