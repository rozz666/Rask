// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_EXPRESSION_HPP
#define RASK_CST_EXPRESSION_HPP

#include <boost/variant.hpp>
#include <rask/cst/Constant.hpp>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cst
{

typedef boost::variant<Constant, Identifier> Expression;

inline Constant& getConstant(Expression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const Expression& e) { return boost::get<Constant>(e); }

inline Identifier& getIdentifier(Expression& e) { return boost::get<Identifier>(e); }
inline const Identifier& getIdentifier(const Expression& e) { return boost::get<Identifier>(e); }

}
}

#endif // RASK_CST_EXPRESSION_HPP
