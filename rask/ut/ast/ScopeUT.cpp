// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/ast/Scope.hpp>
#include <rask/test/VariableFactory.hpp>

namespace tut
{

struct Scope_TestData
{
    rask::test::VariableFactory variableFactory;
    std::string var1Name;
    std::string var2Name;
    rask::ast::SharedVariable var1a;
    rask::ast::SharedVariable var1b;
    rask::ast::SharedVariable var2;
    rask::ast::Scope scope;

    Scope_TestData()
        : var1Name("asia"), var2Name("kasia"),
        var1a(variableFactory.createShared(var1Name)),
        var1b(variableFactory.createShared(var1Name)),
        var2(variableFactory.createShared(var2Name)) { }

};

typedef test_group<Scope_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Scope");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    ENSURE(scope.addVariable(var1a) == var1a);
    ENSURE(scope.addVariable(var2) == var2);
    ENSURE(*scope.getVariable(var1Name) == var1a);
    ENSURE(*scope.getVariable(var2Name) == var2);
}

template <>
template <>
void object::test<2>()
{
    ENSURE(!scope.getVariable("x"));
}

template <>
template <>
void object::test<3>()
{
    ENSURE(scope.addVariable(var1a) == var1a);
    ENSURE(scope.addVariable(var1b) == var1a);
    ENSURE(*scope.getVariable(var1Name) == var1a);
}

}
