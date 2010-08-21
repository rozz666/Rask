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
#include <rask/ast/Function.hpp>

namespace tut
{

struct Function_TestData
{
};

typedef test_group<Function_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Function");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ensure_equals(ast::Function().valueCount(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::Function f;
    f.addValue(1);

    ensure_equals("count 1", f.valueCount(), 1u);
    ensure_equals("value 1", f.value(0), 1);

    f.addValue(5);

    ensure_equals("count 2", f.valueCount(), 2u);
    ensure_equals("value 2", f.value(0), 1);
    ensure_equals("value 3", f.value(1), 5);
}

}
