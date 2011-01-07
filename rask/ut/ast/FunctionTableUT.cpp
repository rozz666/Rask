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
#include <boost/assign/list_of.hpp>

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

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    using boost::assign::list_of;

    std::string name("fam1");
    ast::Function::ArgumentTypes args1 = list_of(ast::INT32)(ast::INT32);
    ast::SharedCustomFunction fam11 = functionFactory.createShared(name, ast::VOID, args1);
    ast::Function::ArgumentTypes args2 = list_of(ast::BOOLEAN)(ast::INT32);
    ast::SharedCustomFunction fam12 = functionFactory.createShared(name, ast::VOID, args2);
    ast::Function::ArgumentTypes args3 = list_of(ast::INT32)(ast::BOOLEAN);
    ast::SharedCustomFunction fam13 = functionFactory.createShared(name, ast::VOID, args3);

    ENSURE(ft.add(fam11) == fam11);
    ENSURE(ft.add(fam12) == fam12);
    ENSURE(ft.add(fam13) == fam13);
    boost::optional<ast::SharedFunctionFamily> optFam1 = ft.getFamily(name);
    ENSURE(optFam1);
    ast::SharedFunctionFamily fam1 = *optFam1;
    ENSURE_EQUALS(fam1->name(), name);
    ENSURE(fam1->getFunction(args1));
    ENSURE(*fam1->getFunction(args1) == fam11);
    ENSURE(fam1->getFunction(args2));
    ENSURE(*fam1->getFunction(args2) == fam12);
    ENSURE(fam1->getFunction(args3));
    ENSURE(*fam1->getFunction(args3) == fam13);
}

template <>
template <>
void object::test<5>()
{
    ENSURE(!ft.getFamily("xxx"));
}

}
