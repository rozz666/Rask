// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/operatorName.hpp>
#include <gtest/gtest.h>

using namespace rask;
using ast::operatorName;
using cst::BinaryOperator;
using cst::UnaryOperator;


TEST(rask_ast_operatorName, binaryMinus)
{
    ASSERT_EQ(BINARY_MINUS_NAME, operatorName(BinaryOperator::MINUS));
}

TEST(rask_ast_operatorName, binaryPlus)
{
    ASSERT_EQ(BINARY_PLUS_NAME, operatorName(BinaryOperator::PLUS));
}

TEST(rask_ast_operatorName, unaryMinus)
{
    ASSERT_EQ(UNARY_MINUS_NAME, operatorName(UnaryOperator::MINUS));
}

TEST(rask_ast_operatorName, binaryMult)
{
    ASSERT_EQ(BINARY_MULT_NAME, operatorName(BinaryOperator::MULT));
}

TEST(rask_ast_operatorName, binaryDiv)
{
    ASSERT_EQ(BINARY_DIV_NAME, operatorName(BinaryOperator::DIV));
}

TEST(rask_ast_operatorName, binaryMod)
{
    ASSERT_EQ(BINARY_MOD_NAME, operatorName(BinaryOperator::MOD));
}
