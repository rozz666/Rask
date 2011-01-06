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
#include <rask/null.hpp>

namespace
{
    
MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable& st)
    : rask::ast::Builder(logger, st) { }

    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildFunctionCall,
        (const rask::cst::FunctionCall&, fc)(rask::ast::SharedScope, scope))
    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildUnaryOperatorCall,
        (const rask::cst::UnaryOperatorCall&, oc)(rask::ast::SharedScope, scope))
    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildChainExpression,
        (const rask::cst::ChainExpression&, oc)(rask::ast::SharedScope, scope))
};
    
}

namespace tut
{

struct buildExpression_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;
    rask::ast::SharedScope scope;
    
    buildExpression_TestData() : builder(logger, st), scope(new rask::ast::Scope) { }
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
    boost::optional<ast::Expression> expr = builder.buildExpression(c, null);
    
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
    scope->addVariable(var);
    
    boost::optional<ast::Expression> expr = builder.buildExpression(id, scope);
    
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
    
    ENSURE(!builder.buildExpression(id, scope));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::unknownIdentifier(id.position, id.value));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cst::Expression fc = cst::FunctionCall();
    unsigned n = 5;
    
    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(n)));
    
    boost::optional<ast::Expression> expr = builder.buildExpression(fc, scope);
    
    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getFunctionCall(*expr).args().size() == n);
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(fc), scope));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    
    cst::FunctionCall fc;
    
    MOCK_RETURN(builder, buildFunctionCall, boost::none);
    
    ENSURE(!builder.buildExpression(fc, null));
    ENSURE(logger.errors().empty());
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    
    cst::Expression c = cst::UnaryOperatorCall();
    unsigned n = 5;
    
    MOCK_RETURN(builder, buildUnaryOperatorCall, ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(n)));
    
    boost::optional<ast::Expression> expr = builder.buildExpression(c, scope);
    
    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getFunctionCall(*expr).args().size() == n);
    ENSURE_CALL(builder, buildUnaryOperatorCall(getUnaryOperatorCall(c), scope));
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;
    
    cst::UnaryOperatorCall c;
    
    MOCK_RETURN(builder, buildUnaryOperatorCall, boost::none);
    
    ENSURE(!builder.buildExpression(c, null));
    ENSURE(logger.errors().empty());
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;

    cst::Expression ce = cst::ChainExpression();

    MOCK_RETURN(builder, buildChainExpression, boost::none);

    ENSURE(!builder.buildExpression(ce, scope));
    ENSURE(logger.errors().empty());
    ENSURE_CALL(builder, buildChainExpression(getChainExpression(ce), scope));
}

template <>
template <>
void object::test<9>()
{
    using namespace rask;

    ast::Constant ret(5);
    cst::ChainExpression ce;

    MOCK_RETURN(builder, buildChainExpression, ast::Expression(ret));

    boost::optional<ast::Expression> expr = builder.buildExpression(ce, null);

    ENSURE(expr);
    ENSURE(logger.errors().empty());
    ENSURE(getConstant(*expr) == ret);
}

template <>
template <>
void object::test<10>()
{
    using namespace rask;
    
    cst::Identifier c1 = cst::Identifier::create(Position(), "true");
    cst::Identifier c2 = cst::Identifier::create(Position(), "false");
    boost::optional<ast::Expression> expr1 = builder.buildExpression(c1, null);
    boost::optional<ast::Expression> expr2 = builder.buildExpression(c2, null);
    
    ENSURE(expr1);
    ENSURE(expr2);
    ENSURE(logger.errors().empty());
    ENSURE(getConstant(*expr1) == ast::Constant(true));
    ENSURE(getConstant(*expr2) == ast::Constant(false));
}

}
