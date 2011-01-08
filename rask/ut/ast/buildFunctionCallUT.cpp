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
#include <rask/ast/BuiltinFunction.hpp>
#include <rask/ut/ast/ScopeMock.hpp>

namespace
{

MOCK(BuilderMock, rask::ast::Builder)
{
    BuilderMock(rask::error::Logger& logger, rask::ast::FunctionTable& ft)
    : rask::ast::Builder(logger, ft) { }

    MOCK_METHOD(boost::optional<rask::ast::Expression>, buildExpression,
        (const rask::cst::Expression&, expr)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildFunctionCall_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::ast::FunctionTable ft;
    BuilderMock builder;
    rask::cst::FunctionCall ccall;
    rask::ast::Constant dummy1;
    rask::ast::Constant dummy2;
    rask::ast::test::SharedScopeMock scope;

    buildFunctionCall_TestData()
        : file("test.rask"), builder(logger, ft), dummy1(1), dummy2(2),
        scope(new rask::ast::test::ScopeMock) { }
};

typedef test_group<buildFunctionCall_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildFunctionCall");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("f", ast::VOID, 0));
    ft.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall, scope);

    ENSURE(call);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE(call->function().lock() == f);
    ENSURE_EQUALS(call->args().size(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("f", ast::VOID, 2));
    ft.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    ccall.args.resize(2);

    MOCK_RETURN(builder, buildExpression, ast::Expression(dummy1));
    MOCK_RETURN(builder, buildExpression, ast::Expression(dummy2));

    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall, scope);

    ENSURE(call);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE(call->function().lock() == f);
    ENSURE_EQUALS(call->args().size(), 2u);
    ENSURE_CALL(builder, buildExpression(ccall.args[0], scope));
    ENSURE(getConstant(call->args()[0]) == dummy1);
    ENSURE_CALL(builder, buildExpression(ccall.args[1], scope));
    ENSURE(getConstant(call->args()[1]) == dummy2);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ccall.function = cst::Identifier::create(Position(file, 2, 4), "xxx");
    ccall.args.resize(1);

    ENSURE(!builder.buildFunctionCall(ccall, scope));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::unknownIdentifier(ccall.function.position, ccall.function.value));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("abc", ast::VOID, 1));
    ft.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    ENSURE(!builder.buildFunctionCall(ccall, scope));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::functionNotFound(ccall.function.position, "abc()"));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("print", ast::VOID, 1));
    ft.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "print");
    ccall.args.resize(2);

    MOCK_RETURN(builder, buildExpression, ast::Expression(dummy1));
    MOCK_RETURN(builder, buildExpression, ast::Expression(dummy2));

    ENSURE(!builder.buildFunctionCall(ccall, scope));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::functionNotFound(ccall.function.position, "print(int32, int32)"));
    ENSURE_CALL(builder, buildExpression(ccall.args[0], scope));
    ENSURE_CALL(builder, buildExpression(ccall.args[1], scope));
}

}
