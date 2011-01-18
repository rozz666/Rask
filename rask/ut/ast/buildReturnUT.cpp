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
#include <rask/ut/ast/ScopeMock.hpp>
#include <rask/null.hpp>

using namespace rask;

namespace
{

CLASS_MOCK(BuilderMock, rask::ast::Builder)
{
    BuilderMock() : rask::ast::Builder(null, null, null) { }

    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildExpression,
        (const rask::cst::Expression&, expr)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildReturn_TestData
{
    rask::cst::Return ret;
    rask::ast::test::SharedScopeMock scope;
    BuilderMock builder;

    buildReturn_TestData() : scope(new rask::ast::test::ScopeMock) { }
};

typedef test_group<buildReturn_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildReturn");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::Constant value(555);

    MOCK_RETURN(builder, buildExpression, ast::Expression(value));

    boost::optional<ast::Return> r = builder.buildReturn(ret, scope);

    ENSURE(r);
    ENSURE_CALL(builder, buildExpression(ret.value, scope));
    ENSURE(getConstant(r->expr()) == value);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    MOCK_RETURN(builder, buildExpression, boost::none);

    ENSURE(!builder.buildReturn(ret, scope));
    ENSURE_CALL(builder, buildExpression(ret.value, scope));
}

}
