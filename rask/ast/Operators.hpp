// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_OPERATORS_HPP
#define RASK_AST_OPERATORS_HPP
#include <rask/cst/Expression.hpp>

namespace rask
{
namespace ast
{

inline std::string operatorName(const cst::BinaryOperator::Tag& op)
{
    return (op == cst::BinaryOperator::MINUS) ? "operator-" : "operator+";
}

inline std::string operatorName(cst::UnaryOperator::Tag)
{
    return "operator-";
}

}
}
#endif /* RASK_AST_OPERATORS_HPP */