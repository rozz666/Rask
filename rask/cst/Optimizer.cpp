// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <rask/cst/Optimizer.hpp>

namespace rask
{
namespace cst
{

void Optimizer::optimize(ChainExpression& ce) const
{
    if (ce.expr.type() == typeid(ChainExpression))
    {
        if (ce.next.empty())
        {
            do
            {
                ce = ChainExpression(getChainExpression(ce.expr));
            }
            while (ce.next.empty());
        }
        else
        {
            ce.expr = Expression(getChainExpression(ce.expr).expr);
        }
    }
}

}
}

