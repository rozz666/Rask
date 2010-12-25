// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_OPERATORNAME_HPP
#define RASK_AST_OPERATORNAME_HPP
#include <rask/cst/Expression.hpp>
#include <rask/Operators.hpp>

namespace rask
{
namespace ast
{

inline std::string operatorName(cst::BinaryOperator::Tag op)
{
    return (op == cst::BinaryOperator::MINUS) ? BINARY_MINUS_NAME : BINARY_PLUS_NAME;
}

inline std::string operatorName(cst::UnaryOperator::Tag)
{
    return UNARY_MINUS_NAME;
}

}
}
#endif /* RASK_AST_OPERATORNAME_HPP */