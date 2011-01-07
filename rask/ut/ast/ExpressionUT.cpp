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
#include <rask/test/FunctionFactory.hpp>

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

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::BasicType type1 = ast::INT32;
    ast::BasicType type2 = ast::BOOLEAN;
    ast::SharedFunction f1 = test::FunctionFactory().createShared("f1", type1, 0);
    ast::FunctionCall fc1(f1, ast::FunctionCall::Arguments());
    ast::SharedFunction f2 = test::FunctionFactory().createShared("f2", type2, 0);
    ast::FunctionCall fc2(f2, ast::FunctionCall::Arguments());

    ENSURE(ast::getExpressionType(ast::Expression(fc1)) == type1);
    ENSURE(ast::getExpressionType(ast::Expression(fc2)) == type2);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ENSURE(ast::getExpressionType(ast::Expression(ast::Constant(false))) == ast::BOOLEAN);
    ENSURE(ast::getExpressionType(ast::Expression(ast::Constant(boost::int32_t(7)))) == ast::INT32);
}

}
