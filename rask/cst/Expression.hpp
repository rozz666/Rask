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

}
}

#endif // RASK_CST_EXPRESSION_HPP
