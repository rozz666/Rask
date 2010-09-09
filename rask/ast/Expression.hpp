// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_EXPRESSION_HPP
#define RASK_AST_EXPRESSION_HPP

#include <boost/variant.hpp>
#include <rask/ast/Constant.hpp>
#include <rask/ast/Variable.hpp>

namespace rask
{
namespace ast
{

typedef boost::variant<Constant, WeakVariable> Expression;

inline Constant& getConstant(Expression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const Expression& e) { return boost::get<Constant>(e); }

inline WeakVariable& getVariable(Expression& e) { return boost::get<WeakVariable>(e); }
inline const WeakVariable& getVariable(const Expression& e) { return boost::get<WeakVariable>(e); }

}

}

#endif // RASK_AST_EXPRESSION_HPP
