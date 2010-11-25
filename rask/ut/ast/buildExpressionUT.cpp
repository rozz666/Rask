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
};

}

namespace tut
{

struct buildExpression_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable dummySt, st;
    BuilderMock builder;

    buildExpression_TestData() : builder(logger, dummySt) { }
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

    cst::Constant c = cst::Constant::create(Position(), 123);
    boost::optional<ast::Expression> expr = builder.buildExpression(c, st);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getConstant(*expr) == ast::Constant(c.value));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::Identifier id = cst::Identifier::create(Position(), "abc");
    ast::SharedVariable var(new ast::Variable(id));
    st.add(var);
    
    boost::optional<ast::Expression> expr = builder.buildExpression(id, st);
    
    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getVariable(*expr).lock() == var);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cst::Identifier id = cst::Identifier::create(Position("xxx", 1, 2), "abc");
    
    ENSURE(!builder.buildExpression(id, st));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::unknownIdentifier(id.position, id.value));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    cst::FunctionCall fc;
    unsigned n = 5;

    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(n)));

    boost::optional<ast::Expression> expr = builder.buildExpression(fc, st);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getFunctionCall(*expr).args().size() == n);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    cst::FunctionCall fc;

    MOCK_RETURN(builder, buildFunctionCall, boost::none);

    ENSURE(!builder.buildExpression(fc, st));
    ENSURE(logger.errors().empty());
}

}
