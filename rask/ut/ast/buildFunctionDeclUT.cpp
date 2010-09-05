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
#include <rask/ast/Builder.hpp>

namespace tut
{

struct buildFunctionDecl_TestData
{
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

    error::Logger logger;
    ast::SymbolTable st;
    ast::Builder builder(logger, st);

    cst::Function cf;
    cf.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf);

    ensure("built", fd);
    ensure_equals("no errors", logger.errors().size(), 0u);
    
    ast::SharedFunction f = fd->function();
    ensure("st", st.getFunction(f->name().value) == f);
    ensure_equals("name pos", f->name().position, cf.name.position);
    ensure_equals("name value", f->name().value, cf.name.value);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    error::Logger logger;
    ast::SymbolTable st;
    ast::Builder builder(logger, st);
    
    cst::Function cf1;
    cf1.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");
    cst::Function cf2;
    cf2.name = cst::Identifier::create(Position("xxx", 10, 2), "asia");
    
    builder.buildFunctionDecl(cf1);
    
    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf2);
    
    ensure_not("not built", fd);
    ensure_equals("errors", logger.errors().size(), 2u);
    ensure_equals("error 1", logger.errors()[0], error::Message::redefinition(cf2.name.position, "asia()"));
    ensure_equals("error 2", logger.errors()[1], error::Message::previousDefinition(cf1.name.position, "asia()"));
}

}
