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
#include <rask/test/VariableFactory.hpp>

namespace tut
{

struct AstSymbolTable_TestData
{
    rask::ast::SymbolTable st;
    rask::test::FunctionFactory functionFactory;
    rask::test::VariableFactory variableFactory;
    std::string var1Name;
    std::string var2Name;
    rask::ast::SharedVariable var1a;
    rask::ast::SharedVariable var1b;
    rask::ast::SharedVariable var2;
    std::string f1Name;
    std::string f2Name;
    rask::ast::SharedFunction f1a;
    rask::ast::SharedFunction f1b;
    rask::ast::SharedFunction f2;

    AstSymbolTable_TestData()
        : var1Name("asia"), var2Name("kasia"),
        var1a(variableFactory.createShared(var1Name)),
        var1b(variableFactory.createShared(var1Name)),
        var2(variableFactory.createShared(var2Name)),
        f1Name("asia"), f2Name("kasia"),
        f1a(functionFactory.createShared(f1Name)),
        f1b(functionFactory.createShared(f1Name)),
        f2(functionFactory.createShared(f2Name))
    {
    }
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
    ENSURE(st.add(var1a) == var1a);
    ENSURE(st.add(var2) == var2);
    ENSURE(*st.getVariable(var1Name) == var1a);
    ENSURE(*st.getVariable(var2Name) == var2);
}

template <>
template <>
void object::test<2>()
{
    ENSURE(!st.getVariable("x"));
}

template <>
template <>
void object::test<3>()
{
    ENSURE(st.add(f1a) == f1a);
    ENSURE(st.add(f2) == f2);
    ENSURE(*st.getFunction(f1Name) == f1a);
    ENSURE(*st.getFunction(f2Name) == f2);
}

template <>
template <>
void object::test<4>()
{
    ENSURE(!st.getFunction("x"));
}

template <>
template <>
void object::test<5>()
{
    ENSURE(st.add(var1a) == var1a);
    ENSURE(st.add(var1b) == var1a);
    ENSURE(*st.getVariable(var1Name) == var1a);
}

template <>
template <>
void object::test<6>()
{
    ENSURE(st.add(f1a) == f1a);
    ENSURE(st.add(f1b) == f1a);
    ENSURE(st.getFunction(f1Name) == f1a);
}

template <>
template <>
void object::test<7>()
{
    st.add(var1a);
    st.enterScope();

    ENSURE(st.add(var1b) == var1b);
    ENSURE(*st.getVariable(var1Name) == var1b);
}

template <>
template <>
void object::test<8>()
{
    st.add(var1a);
    st.enterScope();
    st.add(var1b);
    st.exitScope();

    ENSURE(*st.getVariable(var1Name) == var1a);
}

template <>
template <>
void object::test<9>()
{
    st.enterScope();
    st.add(var1a);
    st.exitScope();
    st.enterScope();

    ENSURE(!st.getVariable(var1Name));
}

template <>
template <>
void object::test<10>()
{
    ENSURE_THROWS(st.exitScope(), std::out_of_range);
    st.enterScope();
    st.exitScope();
    ENSURE_THROWS(st.exitScope(), std::out_of_range);
}


}
