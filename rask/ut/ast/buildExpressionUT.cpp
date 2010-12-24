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

namespace
{

MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable& st)
        : rask::ast::Builder(logger, st) { }

    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildFunctionCall, (const rask::cst::FunctionCall&, fc));
    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildUnaryOperatorCall, (const rask::cst::UnaryOperatorCall&, oc));
    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildUnaryExpression, (const rask::cst::UnaryExpression&, expr));
};

}

namespace tut
{

struct buildExpression_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;

    buildExpression_TestData() : builder(logger, st) { }

    rask::cst::Expression createExpression(const rask::cst::UnaryExpression& u)
    {
        rask::cst::Expression e;
        e.expr = u;
        return e;
    }
};

typedef test_group<buildExpression_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildExpression");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Expression e;
    ast::Constant c(7);

    MOCK_RETURN(builder, buildUnaryExpression, ast::Expression(c));
    boost::optional<ast::Expression> expr = builder.buildExpression(e);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getConstant(*expr) == c);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cst::Expression e;
    
    MOCK_RETURN(builder, buildUnaryExpression, boost::none);
    ENSURE(!builder.buildExpression(e));
    ENSURE(logger.errors().empty());
}

}
