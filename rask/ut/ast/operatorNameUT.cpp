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
#include <rask/ast/operatorName.hpp>

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
tut::factory tf("rask.ast.operatorName");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::MINUS), rask::BINARY_MINUS_NAME);
}

template <>
template <>
void object::test<2>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::PLUS), rask::BINARY_PLUS_NAME);
}

template <>
template <>
void object::test<3>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::UnaryOperator::MINUS), rask::UNARY_MINUS_NAME);
}

template <>
template <>
void object::test<4>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::MULT), rask::BINARY_MULT_NAME);
}

template <>
template <>
void object::test<5>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::DIV), rask::BINARY_DIV_NAME);
}

template <>
template <>
void object::test<6>()
{
    ENSURE_EQUALS(rask::ast::operatorName(rask::cst::BinaryOperator::MOD), rask::BINARY_MOD_NAME);
}

}
