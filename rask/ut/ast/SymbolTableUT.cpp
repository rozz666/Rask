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
#include <rask/ast/SymbolTable.hpp>

namespace tut
{

struct AstSymbolTable_TestData
{
    rask::ast::SymbolTable st;
};

typedef test_group<AstSymbolTable_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.SymbolTable");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::SharedVariable var1(new ast::Variable(cst::Identifier::create(Position(), "asia")));
    ast::SharedVariable var2(new ast::Variable(cst::Identifier::create(Position(), "kasia")));

    ensure("add 1", st.add(var1) == var1);
    ensure("add 2", st.add(var2) == var2);
    
    ensure("var1", st.getVariable(var1->name().value) == var1);
    ensure("var2", st.getVariable(var2->name().value) == var2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    try
    {
        st.getVariable("x");
        fail("expected SymbolTableError");
    }
    catch (const ast::SymbolTableError& e)
    {
        ensure_equals(e.what(), std::string("Symbol \'x\' not found"));
    }
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    ast::SharedFunction f1(new ast::CustomFunction(cst::Identifier::create(Position(), "asia")));
    ast::SharedFunction f2(new ast::CustomFunction(cst::Identifier::create(Position(), "kasia")));
    
    ensure("add 1", st.add(f1) == f1);
    ensure("add 2", st.add(f2) == f2);
    
    ensure("f1", st.getFunction(f1->name().value) == f1);
    ensure("f2", st.getFunction(f2->name().value) == f2);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    try
    {
        st.getFunction("x");
        fail("expected SymbolTableError");
    }
    catch (const ast::SymbolTableError& e)
    {
        ensure_equals(e.what(), std::string("Symbol \'x\' not found"));
    }
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    
    ast::SharedVariable var1(new ast::Variable(cst::Identifier::create(Position("", 1, 2), "asia")));
    ast::SharedVariable var2(new ast::Variable(cst::Identifier::create(Position("", 2, 3), "asia")));
    
    ensure("add 1", st.add(var1) == var1);
    ensure("add 2", st.add(var2) == var1);
    
    ensure("var1", st.getVariable(var1->name().value) == var1);
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    
    ast::SharedFunction f1(new ast::CustomFunction(cst::Identifier::create(Position("", 1, 2), "asia")));
    ast::SharedFunction f2(new ast::CustomFunction(cst::Identifier::create(Position("", 3, 4), "asia")));
    
    ensure("add 1", st.add(f1) == f1);
    ensure("add 2", st.add(f2) == f1);
    
    ensure("f1", st.getFunction(f1->name().value) == f1);
}

}
