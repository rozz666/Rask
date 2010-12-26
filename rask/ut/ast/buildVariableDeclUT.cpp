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

    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildChainExpression, (const rask::cst::ChainExpression&, expr));
};
    
}

namespace tut
{

struct buildVariableDecl_TestData
{
    rask::cst::VariableDecl cvd;
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;
    
    buildVariableDecl_TestData() : builder(logger, st)
    {
        cvd.name = rask::cst::Identifier::create(rask::Position("abc", 1, 3), "x");
        cvd.value = rask::cst::ChainExpression();
    }
};

typedef test_group<buildVariableDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildVariableDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cvd.value = cst::ChainExpression();

    rask::ast::Constant dummy(123);
    MOCK_RETURN(builder, buildChainExpression, ast::Expression(dummy));

    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd);

    ENSURE(vd);
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildChainExpression(*cvd.value));
    ENSURE_EQUALS(vd->var()->name().position, cvd.name.position);
    ENSURE_EQUALS(vd->var()->name().value, cvd.name.value);
    ENSURE_EQUALS(getConstant(vd->value()), dummy);
    ENSURE(st.getVariable(cvd.name.value) == vd->var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cvd.value = boost::none;

    ENSURE(!builder.buildVariableDecl(cvd));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::uninitializedVariable(cvd.name.position, cvd.name.value));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    MOCK_RETURN(builder, buildChainExpression, boost::none);

    ENSURE(!builder.buildVariableDecl(cvd));
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildChainExpression(*cvd.value));
}

}
