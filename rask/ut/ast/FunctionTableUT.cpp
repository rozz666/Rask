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
#include <rask/ast/FunctionTable.hpp>
#include <rask/test/FunctionFactory.hpp>

namespace tut
{

struct AstSymbolTable_TestData
{
    rask::ast::FunctionTable ft;
    rask::test::FunctionFactory functionFactory;
    std::string f1Name;
    std::string f2Name;
    rask::ast::SharedFunction f1a;
    rask::ast::SharedFunction f1b;
    rask::ast::SharedFunction f2;

    AstSymbolTable_TestData()
        : f1Name("asia"), f2Name("kasia"),
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
    ENSURE(ft.add(f1a) == f1a);
    ENSURE(ft.add(f2) == f2);
    ENSURE(*ft.getFunction(f1Name) == f1a);
    ENSURE(*ft.getFunction(f2Name) == f2);
}

template <>
template <>
void object::test<2>()
{
    ENSURE(!ft.getFunction("x"));
}

template <>
template <>
void object::test<3>()
{
    ENSURE(ft.add(f1a) == f1a);
    ENSURE(ft.add(f1b) == f1a);
    ENSURE(ft.getFunction(f1Name) == f1a);
}

}
