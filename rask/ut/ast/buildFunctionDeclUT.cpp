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

}

namespace tut
{

struct buildFunctionDecl_TestData
{
    rask::error::Logger logger;
    rask::ast::FunctionTable ft;
    rask::ast::Builder builder;
    VariableFactoryMock variableFactory;

    buildFunctionDecl_TestData() : builder(logger, ft) { }
};

typedef test_group<buildFunctionDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildFunctionDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Function cf;
    cf.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");
    cf.type = cst::Identifier::create(Position(), "void");

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf, variableFactory);

    ENSURE(fd);
    ENSURE_EQUALS(logger.errors().size(), 0u);

    ast::SharedFunction f = fd->function();
    ENSURE(ft.getFunction(f->name().value) == f);
    ENSURE_EQUALS(f->name().position, cf.name.position);
    ENSURE_EQUALS(f->name().value, cf.name.value);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::Function cf1;
    cf1.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");
    cf1.type = cst::Identifier::create(Position(), "void");
    cst::Function cf2;
    cf2.name = cst::Identifier::create(Position("xxx", 10, 2), "asia");
    cf2.type = cst::Identifier::create(Position(), "void");

    builder.buildFunctionDecl(cf1, variableFactory);

    // FIXME: depends on previous call, mock FunctionTable instead
    ENSURE(!builder.buildFunctionDecl(cf2, variableFactory));
    ENSURE_EQUALS(logger.errors().size(), 2u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::redefinition(cf2.name.position, "asia()"));
    ENSURE_EQUALS(logger.errors()[1], error::Message::previousDefinition(cf1.name.position, "asia()"));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    cst::Function cf;
    cf.name = cst::Identifier::create(Position(), "asia");
    cf.type = cst::Identifier::create(Position(), "void");
    cf.args.resize(2);
    cf.args[0].name = cst::Identifier::create(Position(), "arg1");
    cf.args[0].type = cst::Identifier::create(Position(), "int32");
    cf.args[1].name = cst::Identifier::create(Position(), "arg2");
    cf.args[1].type = cst::Identifier::create(Position(), "boolean");

    test::VariableFactory testFactory;
    ast::SharedVariable v1 = testFactory.createShared("x");
    ast::SharedVariable v2 = testFactory.createShared("y");
    MOCK_RETURN(variableFactory, createVariable, v1);
    MOCK_RETURN(variableFactory, createVariable, v2);

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf, variableFactory);

    ENSURE(fd);
    ENSURE_EQUALS(logger.errors().size(), 0u);

    ast::SharedCustomFunction f = fd->function();
    ENSURE_EQUALS(f->argCount(), 2u);
    ENSURE(f->arg(0) == v1);
    ENSURE(f->arg(1) == v2);
    ENSURE_CALL(variableFactory, createVariable(cf.args[0].name, ast::INT32));
    ENSURE_CALL(variableFactory, createVariable(cf.args[1].name, ast::BOOLEAN));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    cst::Function cf1;
    cf1.name = cst::Identifier::create(Position(), "asia1");
    cf1.type = cst::Identifier::create(Position(), "void");
    cst::Function cf2;
    cf2.name = cst::Identifier::create(Position(), "asia2");
    cf2.type = cst::Identifier::create(Position(), "int32");
    cst::Function cf3;
    cf3.name = cst::Identifier::create(Position(), "asia3");
    cf3.type = cst::Identifier::create(Position(), "boolean");

    boost::optional<ast::FunctionDecl> fd1 = builder.buildFunctionDecl(cf1, variableFactory);
    boost::optional<ast::FunctionDecl> fd2 = builder.buildFunctionDecl(cf2, variableFactory);
    boost::optional<ast::FunctionDecl> fd3 = builder.buildFunctionDecl(cf3, variableFactory);

    ENSURE(fd1);
    ENSURE(fd2);
    ENSURE(fd3);
    ENSURE_EQUALS(logger.errors().size(), 0u);

    ENSURE(fd1->function()->type() == ast::VOID);
    ENSURE(fd2->function()->type() == ast::INT32);
    ENSURE(fd3->function()->type() == ast::BOOLEAN);
}

}
