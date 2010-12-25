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
#include <rask/ast/Operators.hpp>

namespace tut
{

struct Operators_TestData
{
};

typedef test_group<Operators_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Operators");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::MINUS), "operator-");
}

template <>
template <>
void object::test<2>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::PLUS), "operator+");
}

}
