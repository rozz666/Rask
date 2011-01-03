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
#include <boost/concept_check.hpp>

namespace
{
    
MOCK(BuilderMock, rask::ast::Builder)
{
public:

    rask::ast::SharedCustomFunction main;

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable& st)
        : rask::ast::Builder(logger, st), main(rask::test::FunctionFactory().createShared("main"))
    {
        st.add(main);
    }

    MOCK_METHOD(boost::optional<rask::ast::FunctionDecl>, buildFunctionDecl, (const rask::cst::Function&, f));
    MOCK_METHOD(bool, buildFunction, (const rask::cst::Function&, f));
};

MOCK(BuilderMockBuildFunction, rask::ast::Builder)
{
public:

    rask::ast::SymbolTable& st;
    bool allParamsOk;

    BuilderMockBuildFunction(rask::error::Logger& logger, rask::ast::SymbolTable& st)
        : rask::ast::Builder(logger, st), st(st), allParamsOk(true)
    {
    }

    MOCK_METHOD(boost::optional<rask::ast::FunctionDecl>, buildFunctionDecl, (const rask::cst::Function&, f));

    virtual bool buildFunction(const rask::cst::Function& cf)
    {
        rask::ast::SharedCustomFunction f =
            boost::dynamic_pointer_cast<rask::ast::CustomFunction>(*st.getFunction(cf.name.value));

        if (st.getVariable(f->arg(0)->name().value)) allParamsOk = false;

        return true;
    }
};

}

namespace tut
{

struct buildAST_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    BuilderMock builder;
    rask::cst::Tree cst;

    buildAST_TestData() : builder(logger, st)
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

    boost::optional<ast::Tree> ast = builder.buildTree(cst);

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
    
    boost::optional<ast::Tree> ast = builder.buildTree(cst);
   
    ENSURE(ast);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(ast->functionCount(), 2u);
    ENSURE(ast->function(0) == fd1.function());
    ENSURE(ast->function(1) == fd2.function());
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0]));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1]));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_CALL(builder, buildFunction(cst.functions[0]));
    ENSURE_CALL(builder, buildFunction(cst.functions[1]));
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
    
    ENSURE(!builder.buildTree(cst));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0]));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1]));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cst.functions.resize(2);

    MOCK_RETURN(builder, buildFunctionDecl, ast::FunctionDecl(cst::Identifier::create(Position(), "f"), ast::VOID));
    MOCK_RETURN(builder, buildFunction, false);
    
    ENSURE(!builder.buildTree(cst));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[0]));
    ENSURE_CALL(builder, buildFunctionDecl(cst.functions[1]));
    ENSURE_NO_CALLS(builder, buildFunctionDecl);
    ENSURE_CALL(builder, buildFunction(cst.functions[0]));
    ENSURE_CALL(builder, buildFunction(cst.functions[1]));
    ENSURE_NO_CALLS(builder, buildFunction);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    cst.end = Position("xxx", 1, 2);
    st = ast::SymbolTable();
    
    ENSURE(!builder.buildTree(cst));
    ENSURE_EQUALS(logger.errors().size(), 1u);
    ENSURE_EQUALS(logger.errors()[0], error::Message::missingMainFunction(Position(cst.end.file)));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    rask::ast::SymbolTable st;
    BuilderMockBuildFunction builder(logger, st);

    cst.functions.resize(2);
    cst.functions[0].name.value = "f1";
    cst.functions[0].args.resize(1);
    cst.functions[0].args[0].name = cst::Identifier::create(Position(), "testArg");
    cst.functions[1].name.value = "f2";
    cst.functions[1].args.resize(1);
    cst.functions[1].args[0].name = cst::Identifier::create(Position(), "testArg");

    ast::FunctionDecl fd1(cst::Identifier::create(Position(), cst.functions[0].name.value), ast::VOID);
    ast::FunctionDecl fd2(cst::Identifier::create(Position(), cst.functions[1].name.value), ast::VOID);
    ast::SharedCustomFunction f1 = test::FunctionFactory().createShared("f1");
    f1->addArg(cst.functions[0].args[0].name);
    ast::SharedCustomFunction f2 = test::FunctionFactory().createShared("f2");
    f2->addArg(cst.functions[1].args[0].name);
    st.add(f1);
    st.add(f2);
    MOCK_RETURN(builder, buildFunctionDecl, fd1);
    MOCK_RETURN(builder, buildFunctionDecl, fd2);

    builder.buildTree(cst);

    ENSURE(builder.allParamsOk);
}

}
