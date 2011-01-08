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
#include <rask/test/FunctionFactory.hpp>

namespace
{

MOCK(ScopeFactoryMock, rask::ast::ScopeFactory)
{
    MOCK_METHOD(rask::ast::SharedScope, createScope, )
};

typedef boost::shared_ptr<ScopeFactoryMock> SharedScopeFactoryMock;

MOCK(BuilderMock, rask::ast::Builder)
{
    rask::ast::SharedCustomFunction main;

    BuilderMock(rask::error::Logger& logger, rask::ast::FunctionTable& ft)
        : rask::ast::Builder(logger, ft), main(rask::test::FunctionFactory().createShared("main"))
    {
        ft.add(main);
    }

    MOCK_METHOD(boost::optional<rask::ast::FunctionDecl>, buildFunctionDecl,
        (const rask::cst::Function&, f)(rask::ast::VariableFactory&, variableFactory))
    MOCK_METHOD(bool, buildFunction,
        (const rask::cst::Function&, cf)(rask::ast::SharedCustomFunction, f)(rask::ast::SharedScope, scope))
};

}

namespace tut
{

struct buildAST_TestData
{
    rask::error::Logger logger;
    rask::ast::FunctionTable ft;
    SharedScopeFactoryMock scopeFactory;
    BuilderMock builder;
    rask::cst::Tree cst;

    buildAST_TestData() : scopeFactory(new ScopeFactoryMock), builder(logger, ft)
    {
    }
};

typedef test_group<buildAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildTree");

}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    boost::optional<ast::Tree> ast = builder.buildTree(cst, scopeFactory);

    ENSURE(ast);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(ast->functionCount(), 0u);
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst.functions.resize(2);

    ast::FunctionDecl fd1(cst::Identifier::create(Position(), "f1"), ast::VOID);
    ast::FunctionDecl fd2(cst::Identifier::create(Position(), "f2"), ast::VOID);
    MOCK_RETURN(builder, buildFunctionDecl, fd1);
    MOCK_RETURN(builder, buildFunctionDecl, fd2);
    MOCK_RETURN(builder, buildFunction, true);
    MOCK_RETURN(builder, buildFunction, true);

    ast::SharedScope s1(new ast::Scope);
    ast::SharedScope s2(new ast::Scope);
    MOCK_RETURN(*scopeFactory, createScope, s1);
    MOCK_RETURN(*scopeFactory, createScope, s2);
    boost::optional<ast::Tree> ast = builder.buildTree(cst, scopeFactory);

    ENSURE(ast);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(ast->functionCount(), 2u);
    ENSURE(ast->function(0) == fd1.function());
    ENSURE(ast->function(1) == fd2.function());
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0], builder.variableFactory));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1], builder.variableFactory));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_CALL(builder, buildFunction(cst.functions[0], fd1.function(), s1));
    ENSURE_CALL(builder, buildFunction(cst.functions[1], fd2.function(), s2));
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    cst.functions.resize(2);

    MOCK_RETURN(builder, buildFunctionDecl, ast::FunctionDecl(cst::Identifier::create(Position(), "f"), ast::VOID));
    MOCK_RETURN(builder, buildFunctionDecl, boost::none);

    ENSURE(!builder.buildTree(cst, scopeFactory));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0], builder.variableFactory));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1], builder.variableFactory));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    cst.functions.resize(2);

    ast::FunctionDecl fd(cst::Identifier::create(Position(), "f"), ast::VOID);
    MOCK_RETURN(builder, buildFunctionDecl, fd);
    MOCK_RETURN(builder, buildFunctionDecl, fd);
    MOCK_RETURN(builder, buildFunction, false);
    MOCK_RETURN(builder, buildFunction, false);
    ast::SharedScope s1(new ast::Scope);
    ast::SharedScope s2(new ast::Scope);
    MOCK_RETURN(*scopeFactory, createScope, s1);
    MOCK_RETURN(*scopeFactory, createScope, s2);

    ENSURE(!builder.buildTree(cst, scopeFactory));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0], builder.variableFactory));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1], builder.variableFactory));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_CALL(builder, buildFunction(cst.functions[0], fd.function(), s1));
    ENSURE_CALL(builder, buildFunction(cst.functions[1], fd.function(), s2));
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    cst.end = Position("xxx", 1, 2);
    ft = ast::FunctionTable();

    ENSURE(!builder.buildTree(cst, scopeFactory));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::missingMainFunction(Position(cst.end.file)));
}

}
