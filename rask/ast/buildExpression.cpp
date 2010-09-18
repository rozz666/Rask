// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

struct BuildExpression : boost::static_visitor<boost::optional<Expression> >
{
    SymbolTable& st;

    BuildExpression(SymbolTable& st) : st(st) { }

    Expression operator()(const cst::Constant& c)
    {
        return Constant(c.value);
    }

    Expression operator()(const cst::Identifier& id)
    {
        return st.getVariable(id.value);
    }
};
    
boost::optional<Expression> Builder::buildExpression(const cst::Expression& expr, SymbolTable& st)
{
    BuildExpression b(st);
    return expr.apply_visitor(b);
}

}
}


