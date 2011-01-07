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
#include <rask/ast/Expression.hpp>

namespace tut
{

struct Expression_TestData
{
};

typedef test_group<Expression_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Expression");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::BasicType type1 = ast::INT32;
    ast::BasicType type2 = ast::BOOLEAN;
    ast::SharedVariable v1(new ast::Variable(cst::Identifier(), type1));
    ast::SharedVariable v2(new ast::Variable(cst::Identifier(), type2));

    ENSURE(ast::getExpressionType(ast::Expression(v1)) == type1);
    ENSURE(ast::getExpressionType(ast::Expression(v2)) == type2);
}

}
