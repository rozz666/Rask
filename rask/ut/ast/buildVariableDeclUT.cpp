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
#include <rask/test/VariableFactory.hpp>
#include <rask/ast/Builder.hpp>

namespace
{

MOCK(VariableFactoryMock, rask::ast::VariableFactory)
{
public:

    MOCK_METHOD(rask::ast::SharedVariable, createVariable,
        (const rask::cst::Identifier&, name)(rask::ast::BasicType, type))
};

MOCK(ScopeMock, rask::ast::Scope)
{
public:

    MOCK_METHOD(rask::ast::SharedVariable, addVariable, (rask::ast::SharedVariable, var))
};

MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::FunctionTable& ft)
        : rask::ast::Builder(logger, ft) { }

    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildExpression,
        (const rask::cst::Expression&, expr)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildVariableDecl_TestData
{
    rask::cst::VariableDecl cvd;
    rask::error::Logger logger;
    rask::ast::FunctionTable ft;
    boost::shared_ptr<ScopeMock> scope;
    BuilderMock builder;
    VariableFactoryMock variableFactory;

    buildVariableDecl_TestData()
        : scope(new ScopeMock), builder(logger, ft)
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

    ast::SharedVariable v = test::VariableFactory().createShared("x");
    MOCK_RETURN(variableFactory, createVariable, v);
    MOCK_RETURN(*scope, addVariable, v);
    rask::ast::Constant dummy(123);
    MOCK_RETURN(builder, buildExpression, ast::Expression(dummy));

    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd, scope, variableFactory);

    ENSURE(vd);
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildExpression(*cvd.value, scope));
    ENSURE_CALL(variableFactory, createVariable(cvd.name, dummy.type()));
    ENSURE(vd->var() == v);
    ENSURE(getConstant(vd->value()) == dummy);
    ENSURE_CALL(*scope, addVariable(vd->var()));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cvd.value = boost::none;

    ENSURE(!builder.buildVariableDecl(cvd, scope, variableFactory));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::uninitializedVariable(cvd.name.position, cvd.name.value));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    MOCK_RETURN(builder, buildExpression, boost::none);

    ENSURE(!builder.buildVariableDecl(cvd, scope, variableFactory));
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildExpression(*cvd.value, scope));
}

}
