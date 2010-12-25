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
    switch (op)
    {
        case cst::BinaryOperator::MINUS: return BINARY_MINUS_NAME;
        case cst::BinaryOperator::PLUS: return BINARY_PLUS_NAME;
        default:
            return BINARY_MULT_NAME;
    }
}

inline std::string operatorName(cst::UnaryOperator::Tag)
{
    return UNARY_MINUS_NAME;
}

}
}
#endif /* RASK_AST_OPERATORNAME_HPP */