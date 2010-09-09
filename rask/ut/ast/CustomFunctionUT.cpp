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
#include <rask/ast/CustomFunction.hpp>

namespace
{

struct FunctionTestVisitor : public rask::ast::FunctionVisitor
{
    rask::ast::CustomFunction *f;

    FunctionTestVisitor() : f(0) { }

    virtual void visit(rask::ast::BuiltinFunction& f) { }
    virtual void visit(rask::ast::CustomFunction& f) { this->f = &f; }
};
    
}

namespace tut
{

struct CustomFunction_TestData
{
};

typedef test_group<CustomFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.CustomFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Identifier name = cst::Identifier::create(Position("abc", 10, 20), "kasia");
    ast::CustomFunction cf(name);
    ast::Function& f = cf;
    ast::FunctionCall fc(5);
    ast::VariableDecl vd(cst::Identifier::create(Position("xx", 1, 2), "asia"), 0);
    ensure_equals("no stmts", cf.stmtCount(), 0u);
    cf.addStmt(fc);
    cf.addStmt(vd);
    ensure_equals("name pos", f.name().position, name.position);
    ensure_equals("name val", f.name().value, name.value);
    ensure_equals("arg count", f.argCount(), 0u);
    ensure_equals("count", cf.stmtCount(), 2u);
    ensure_equals("call", boost::get<boost::int32_t>(boost::get<ast::FunctionCall>(cf.stmt(0))), boost::get<boost::int32_t>(fc));
    ensure("var", boost::get<ast::VariableDecl>(cf.stmt(1)).var() == vd.var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::CustomFunction cf(cst::Identifier::create(Position(), "xxx"));
    ast::Function& f = cf;
    FunctionTestVisitor v;
    
    f.accept(v);
    ensure("ok", v.f == &cf);
}
    
}
