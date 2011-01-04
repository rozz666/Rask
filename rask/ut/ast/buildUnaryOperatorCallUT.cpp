// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <rask/Operators.hpp>

namespace
{
    
MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable& st)
        : rask::ast::Builder(logger, st) { }

    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildExpression, (const rask::cst::Expression&, expr));
};
    
}

namespace tut
{

struct buildUnaryOperatorCall_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;

    buildUnaryOperatorCall_TestData() : builder(logger, st) { }
};

typedef test_group<buildUnaryOperatorCall_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildUnaryOperatorCall");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::UnaryOperatorCall oc;
    oc.op.tag = cst::UnaryOperator::MINUS;

    test::FunctionFactory functionFactory;
    ast::SharedCustomFunction f = functionFactory.createShared(UNARY_MINUS_NAME, ast::INT32, 1);
    ast::Constant retExpr(7);
    MOCK_RETURN(builder, buildExpression, ast::Expression(retExpr));

    st.add(f);
    
    boost::optional<ast::FunctionCall> fc = builder.buildUnaryOperatorCall(oc);

    ENSURE(fc);
    ENSURE(fc->function().lock() == f);
    ENSURE_EQUALS(fc->args().size(), 1u);
    ENSURE(getConstant(fc->args()[0]) == retExpr);
}

}
