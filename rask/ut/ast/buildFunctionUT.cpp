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

namespace
{

MOCK(BuilderMock, rask::ast::Builder)
{
public:

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable& st)
        : rask::ast::Builder(logger, st) { }

    MOCK_METHOD(boost::optional<rask::ast::VariableDecl>, buildVariableDecl, (const rask::cst::VariableDecl&, vd));
    MOCK_METHOD(boost::optional<rask::ast::FunctionCall>, buildFunctionCall, (const rask::cst::FunctionCall&, fc));
};
    
}

namespace tut
{

struct buildFunctionAST_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Function cf;
    rask::ast::SharedCustomFunction f;
    rask::ast::SymbolTable st;
    BuilderMock builder;
    
    buildFunctionAST_TestData() : file("test.rask"), builder(logger, st)
    {
        cf.name = rask::cst::Identifier::create(rask::Position(file, 1, 2), "main");
        cf.type = rask::cst::Identifier::create(rask::Position(file, 1, 10), "void");

        f.reset(new rask::ast::CustomFunction(cf.name, rask::ast::VOID));
        
        st.add(f);
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

    ENSURE(builder.buildFunction(cf));
    
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

    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(n1)));
    MOCK_RETURN(builder, buildFunctionCall, ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(n2)));
    
    ENSURE(builder.buildFunction(cf));
    
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_EQUALS(f->stmtCount(), 2u);
    ENSURE_EQUALS(getFunctionCall(f->stmt(0)).args().size(), n1);
    ENSURE_EQUALS(getFunctionCall(f->stmt(1)).args().size(), n2);
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[0])));
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[1])));
    ENSURE_NO_CALLS(builder, buildFunctionCall);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cf.stmts.resize(2, cst::FunctionCall());
    
    MOCK_RETURN(builder, buildFunctionCall, boost::none);
    
    ENSURE(!builder.buildFunction(cf));
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildFunctionCall(getFunctionCall(cf.stmts[0])));
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
    
    MOCK_RETURN(builder, buildVariableDecl, ast::VariableDecl(cst::Identifier::create(Position(), n1), ast::WeakVariable()));
    MOCK_RETURN(builder, buildVariableDecl, ast::VariableDecl(cst::Identifier::create(Position(), n2), ast::WeakVariable()));
    
    ENSURE(builder.buildFunction(cf));
    
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildVariableDecl(getVariableDecl(cf.stmts[0])));
    ENSURE_CALL(builder, buildVariableDecl(getVariableDecl(cf.stmts[1])));
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
    
    ENSURE(!builder.buildFunction(cf));
    
    ENSURE_EQUALS(logger.errors().size(), 0u);
    ENSURE_CALL(builder, buildVariableDecl(boost::get<cst::VariableDecl>(cf.stmts[0])));
    ENSURE_NO_CALLS(builder, buildVariableDecl);
}

}