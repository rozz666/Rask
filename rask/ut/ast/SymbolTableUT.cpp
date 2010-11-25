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
#include <rask/ast/SymbolTable.hpp>
#include <rask/test/FunctionFactory.hpp>

namespace tut
{

struct AstSymbolTable_TestData
{
    rask::ast::SymbolTable st;
    rask::test::FunctionFactory functionFactory;
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

    ENSURE(st.add(var1) == var1);
    ENSURE(st.add(var2) == var2);
    
    ENSURE(*st.getVariable(var1->name().value) == var1);
    ENSURE(*st.getVariable(var2->name().value) == var2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ENSURE(!st.getVariable("x"));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ast::SharedFunction f1 = functionFactory.createShared("asia");
    ast::SharedFunction f2 = functionFactory.createShared("kasia");
    
    ENSURE(st.add(f1) == f1);
    ENSURE(st.add(f2) == f2);
    
    ENSURE(*st.getFunction(f1->name().value) == f1);
    ENSURE(*st.getFunction(f2->name().value) == f2);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    ENSURE(!st.getFunction("x"));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    
    ast::SharedVariable var1(new ast::Variable(cst::Identifier::create(Position("", 1, 2), "asia")));
    ast::SharedVariable var2(new ast::Variable(cst::Identifier::create(Position("", 2, 3), "asia")));
    
    ENSURE(st.add(var1) == var1);
    ENSURE(st.add(var2) == var1);
    
    ENSURE(*st.getVariable(var1->name().value) == var1);
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    
    ast::SharedFunction f1 = functionFactory.createShared(1, 2, "asia");
    ast::SharedFunction f2 = functionFactory.createShared(3, 4, "asia");
    
    ENSURE(st.add(f1) == f1);
    ENSURE(st.add(f2) == f1);
    
    ENSURE(st.getFunction(f1->name().value) == f1);
}

}
