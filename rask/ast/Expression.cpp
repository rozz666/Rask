// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Expression.hpp>

namespace rask
{
namespace ast
{

BasicType getExpressionType(const Expression& e)
{
    return getVariable(e).lock()->type();
}

}
}