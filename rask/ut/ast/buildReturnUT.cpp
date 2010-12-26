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

struct buildReturn_TestData
{
    rask::cst::Return ret;
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;

    buildReturn_TestData() : builder(logger, st) { }
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

    ast::Constant value = 555;

    MOCK_RETURN(builder, buildChainExpression, ast::Expression(value));

    boost::optional<ast::Return> r = builder.buildReturn(ret);

    ENSURE(r);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildChainExpression(ret.value));
    ENSURE(getConstant(r->expr()) == value);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    MOCK_RETURN(builder, buildChainExpression, boost::none);

    ENSURE(!builder.buildReturn(ret));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildChainExpression(ret.value));
}

}
