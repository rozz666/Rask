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
#include <rask/ast/BuiltinFunctions.hpp>
#include <rask/ast/FunctionTable.hpp>
#include <rask/Operators.hpp>

namespace tut
{

struct BuiltinFunctions_TestData
{
    rask::ast::BuiltinFunctions bfs;
    rask::ast::FunctionTable ft;

    BuiltinFunctions_TestData()
    {
        bfs.declare(ft);
    }

    void ensureBuiltinFunction(const std::string& name, rask::ast::BasicType type, unsigned argCount)
    {
        using namespace rask;
        
        boost::optional<ast::SharedFunction> f = ft.getFunction(name);
        ENSURE(f);
        ENSURE(boost::dynamic_pointer_cast<ast::BuiltinFunction>(*f));
        ENSURE_EQUALS((*f)->name().position, Position());
        ENSURE_EQUALS((*f)->name().value, name);
        ENSURE_EQUALS((*f)->argCount(), argCount);
        ENSURE((*f)->type() == type);
    }
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
    ensureBuiltinFunction("print", rask::ast::VOID, 1);
}

template <>
template <>
void object::test<2>()
{
    ensureBuiltinFunction("getInt32", rask::ast::INT32, 0);;
}

template <>
template <>
void object::test<3>()
{
    ensureBuiltinFunction(rask::UNARY_MINUS_NAME, rask::ast::INT32, 1);
}

template <>
template <>
void object::test<4>()
{
    ensureBuiltinFunction(rask::BINARY_MINUS_NAME, rask::ast::INT32, 2);
}

template <>
template <>
void object::test<5>()
{
    ensureBuiltinFunction(rask::BINARY_PLUS_NAME, rask::ast::INT32, 2);
}

template <>
template <>
void object::test<6>()
{
    ensureBuiltinFunction(rask::BINARY_MULT_NAME, rask::ast::INT32, 2);
}

template <>
template <>
void object::test<7>()
{
    ensureBuiltinFunction(rask::BINARY_DIV_NAME, rask::ast::INT32, 2);
}

template <>
template <>
void object::test<8>()
{
    ensureBuiltinFunction(rask::BINARY_MOD_NAME, rask::ast::INT32, 2);
}


}
