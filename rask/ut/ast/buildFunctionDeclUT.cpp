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

namespace tut
{

struct buildFunctionDecl_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    rask::ast::Builder builder;

    buildFunctionDecl_TestData() : builder(logger, st) { } 
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

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf);

    ENSURE(fd);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    
    ast::SharedFunction f = fd->function();
    ENSURE(st.getFunction(f->name().value) == f);
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
    
    builder.buildFunctionDecl(cf1);
    
    ENSURE(!builder.buildFunctionDecl(cf2));
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
    cf.args.push_back(cst::Identifier::create(Position(), "arg1"));
    cf.args.push_back(cst::Identifier::create(Position(), "arg2"));
    
    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf);
    
    ENSURE(fd);
    ENSURE_EQUALS(logger.errors().size(), 0u);
    
    ast::SharedCustomFunction f = fd->function();
    ENSURE_EQUALS(f->argCount(), 2u);
    ENSURE_EQUALS(f->arg(0)->name().value, cf.args[0].value);
    ENSURE_EQUALS(f->arg(1)->name().value, cf.args[1].value);
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

    boost::optional<ast::FunctionDecl> fd1 = builder.buildFunctionDecl(cf1);
    boost::optional<ast::FunctionDecl> fd2 = builder.buildFunctionDecl(cf2);

    ENSURE(fd1);
    ENSURE(fd2);
    ENSURE_EQUALS(logger.errors().size(), 0u);

    ENSURE(fd1->function()->type() == ast::VOID);
    ENSURE(fd2->function()->type() == ast::INT32);
}

}
