// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <rask/ast/operatorName.hpp>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

boost::optional<Expression> Builder::buildChainExpression(const cst::ChainExpression& expr, SharedScope scope)
{
    boost::optional<Expression> left = buildExpression(expr.expr, scope);

    if (!left) return boost::none;

    for(std::size_t i = 0; i != expr.next.size(); ++i)
    {
        boost::optional<Expression> right = buildExpression(expr.next[i].expr, scope);

        if (!right) return boost::none;

        FunctionCall::Arguments args(2);
        args[0] = *left;
        args[1] = *right;

        left = FunctionCall(*functionTable_->getFunction(operatorName(expr.next[i].op.tag)), args);
    }

    return left;
}

}
}
