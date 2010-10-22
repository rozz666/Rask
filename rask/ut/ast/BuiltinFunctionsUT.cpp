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
#include <rask/ast/BuiltinFunctions.hpp>
#include <rask/ast/SymbolTable.hpp>

namespace tut
{

struct BuiltinFunctions_TestData
{
};

typedef test_group<BuiltinFunctions_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.BuiltinFunctions");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::BuiltinFunctions bfs;
    ast::SymbolTable st;

    bfs.declare(st);

    ast::SharedFunction print = *st.getFunction("print");

    ensure("builtin", boost::dynamic_pointer_cast<ast::BuiltinFunction>(print));
    ensure_equals("name", print->name().value, "print");
    ensure_equals("arg count", print->argCount(), 1);
}

}
