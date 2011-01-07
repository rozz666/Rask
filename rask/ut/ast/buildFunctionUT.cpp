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
#include <rask/test/Mock.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/test/VariableDeclFactory.hpp>
#include <rask/null.hpp>

namespace
{

MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::FunctionTable& ft)
        : rask::ast::Builder(logger, ft) { }

    MOCK_METHOD(boost::optional<rask::ast::VariableDecl>, buildVariableDecl,
        (const rask::cst::VariableDecl&, vd)
        (rask::ast::SharedScope, scope)
        (rask::ast::VariableFactory&, variableFactory))
    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildFunctionCall,
        (const rask::cst::FunctionCall&, fc)(rask::ast::SharedScope, scope))
    MOCK_METHOD(boost::optional<rask::ast::Return>, buildReturn,
        (const rask::cst::Return&, ret)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildFunctionAST_TestData
{
    rask::test::VariableDeclFactory varDeclFactory;
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Function cf;
    rask::ast::SharedCustomFunction f;
    rask::ast::FunctionTable ft;
    rask::ast::SharedScope scope;
    BuilderMock builder;

    buildFunctionAST_TestData()
        : file("test.rask"), scope(new rask::ast::Scope), builder(logger, ft)
    {
        cf.name = rask::cst::Identifier::create(rask::Position(file, 1, 2), "main");
        cf.type = rask::cst::Identifier::create(rask::Position(file, 1, 10), "void");

        f.reset(new rask::ast::CustomFunction(cf.name, rask::ast::VOID));
    }
};

typedef test_group<buildFunctionAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ENSURE(builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(f->stmtCount(), 0u);
}


template <>
template <>
void object::test<2>()
{
    using namespace rask;

    const unsigned n1 = 1;
    const unsigned n2 = 2;

    cf.stmts.resize(2, cst::FunctionCall());

    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(null, ast::FunctionCall::Arguments(n1)));
    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(null, ast::FunctionCall::Arguments(n2)));

    ENSURE(builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(f->stmtCount(), 2u);
    ENSURE_EQUALS(getFunctionCall(f->stmt(0)).args().size(), n1);
    ENSURE_EQUALS(getFunctionCall(f->stmt(1)).args().size(), n2);
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[0]), scope));
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[1]), scope));
    ENSURE_NO_CALLS(builder, buildFunctionCall);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    cf.stmts.resize(2, cst::FunctionCall());

    MOCK_RETURN(builder, buildFunctionCall, boost::none);

    ENSURE(!builder.buildFunction(cf, f, scope));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[0]), scope));
    ENSURE_NO_CALLS(builder, buildFunctionCall);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    const std::string n1 = "a";
    const std::string n2 = "b";

    cf.stmts.resize(2, cst::VariableDecl());

    MOCK_RETURN(builder, buildVariableDecl, varDeclFactory.create(n1));
    MOCK_RETURN(builder, buildVariableDecl, varDeclFactory.create(n2));

    ENSURE(builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildVariableDecl(getVariableDecl(cf.stmts[0]), scope, builder.variableFactory));
    ENSURE_CALL(builder, buildVariableDecl(getVariableDecl(cf.stmts[1]), scope, builder.variableFactory));
    ENSURE_NO_CALLS(builder, buildVariableDecl);

    ENSURE_EQUALS(f->stmtCount(), 2u);
    ENSURE_EQUALS(getVariableDecl(f->stmt(0)).var()->name().value, n1);
    ENSURE_EQUALS(getVariableDecl(f->stmt(1)).var()->name().value, n2);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    cf.stmts.resize(2, cst::VariableDecl());

    MOCK_RETURN(builder, buildVariableDecl, boost::none);

    ENSURE(!builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildVariableDecl(boost::get<cst::VariableDecl>(cf.stmts[0]), scope, builder.variableFactory));
    ENSURE_NO_CALLS(builder, buildVariableDecl);
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    ast::Constant c1(1);
    ast::Constant c2(2);

    cf.stmts.resize(2, cst::Return());

    MOCK_RETURN(builder, buildReturn, ast::Return(c1));
    MOCK_RETURN(builder, buildReturn, ast::Return(c2));

    ENSURE(builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildReturn(getReturn(cf.stmts[0]), scope));
    ENSURE_CALL(builder, buildReturn(getReturn(cf.stmts[1]), scope));

    ENSURE_EQUALS(f->stmtCount(), 2u);
    ENSURE(getConstant(getReturn(f->stmt(0)).expr()) == c1);
    ENSURE(getConstant(getReturn(f->stmt(1)).expr()) == c2);
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;

    cf.stmts.resize(2, cst::Return());

    MOCK_RETURN(builder, buildReturn, boost::none);
    MOCK_RETURN(builder, buildReturn, boost::none);

    ENSURE(!builder.buildFunction(cf, f, scope));

    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildReturn(getReturn(cf.stmts[0]), scope));
    ENSURE_NO_CALLS(builder, buildReturn);
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;

    cf.args.resize(2);
    cf.args[0].name = cst::Identifier::create(Position(), "arg1");
    cf.args[1].name = cst::Identifier::create(Position(), "arg2");

    f->addArg(cf.args[0].name);
    f->addArg(cf.args[1].name);

    ENSURE(builder.buildFunction(cf, f, scope));

    ENSURE(scope->getVariable(cf.args[0].name.value) == f->arg(0));
    ENSURE(scope->getVariable(cf.args[1].name.value) == f->arg(1));
}

}