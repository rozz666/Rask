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
#include <boost/assign/list_of.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/ast/CustomFunction.hpp>
#include <rask/test/VariableDeclFactory.hpp>

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
    rask::test::VariableDeclFactory varDeclFactory;
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
    ast::CustomFunction cf(name, ast::VOID);
    ast::Function& f = cf;
    ast::SharedCustomFunction dummy(new ast::CustomFunction(cst::Identifier::create(Position(), "test"), ast::VOID));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());
    ast::VariableDecl vd = varDeclFactory.create("asia");
    ENSURE_EQUALS(cf.stmtCount(), 0u);
    cf.addStmt(fc);
    cf.addStmt(vd);
    ENSURE_EQUALS(f.name().position, name.position);
    ENSURE_EQUALS(f.name().value, name.value);
    ENSURE_EQUALS(f.argCount(), 0u);
    ENSURE_EQUALS(cf.stmtCount(), 2u);
    ENSURE(getFunctionCall(cf.stmt(0)).function().lock() == dummy);
    ENSURE_EQUALS(getFunctionCall(cf.stmt(0)).args().size(), 0u);
    ENSURE(getVariableDecl(cf.stmt(1)).var() == vd.var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::CustomFunction cf(cst::Identifier::create(Position(), "xxx"), ast::VOID);
    ast::Function& f = cf;
    FunctionTestVisitor v;
    
    f.accept(v);
    ENSURE(v.f == &cf);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    ast::CustomFunction cf(cst::Identifier::create(Position(), "xxx"), ast::VOID);
    cst::Identifier arg1 = cst::Identifier::create(Position("a", 1, 2), "arg1");
    cst::Identifier arg2 = cst::Identifier::create(Position("b", 3, 4), "arg2");

    cf.addArg(arg1);
    ENSURE_EQUALS(cf.argCount(), 1u);
    ast::SharedVariable v1 = cf.arg(0);
    ENSURE_EQUALS(v1->name().value, arg1.value);
    ENSURE_EQUALS(v1->name().position, arg1.position);
    cf.addArg(arg2);
    ENSURE_EQUALS(cf.argCount(), 2u);
    ast::SharedVariable v2 = cf.arg(1);
    ENSURE_EQUALS(v2->name().value, arg2.value);
    ENSURE_EQUALS(v2->name().position, arg2.position);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    ast::CustomFunction cf1(cst::Identifier::create(Position(), "abc"), ast::VOID);
    ast::CustomFunction cf2(cst::Identifier::create(Position(), "abc"), ast::INT32);

    ENSURE(cf1.type() == ast::VOID);
    ENSURE(cf2.type() == ast::INT32);
}
    
}
