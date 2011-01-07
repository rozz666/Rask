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

struct GenExpressionType : boost::static_visitor<BasicType>
{
    result_type operator()(const FunctionCall& fc) const
    {
        return fc.function().lock()->type();
    }

    result_type operator()(const WeakVariable& v) const
    {
        return v.lock()->type();
    }

    result_type operator()(const Constant& c) const
    {
        return c.type();
    }
};

BasicType getExpressionType(const Expression& e)
{
    GenExpressionType g;
    return e.apply_visitor(g);
}

}
}