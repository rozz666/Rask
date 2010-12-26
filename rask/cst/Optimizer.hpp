// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_OPTIMIZER_HPP
#define RASK_CST_OPTIMIZER_HPP

#include <rask/cst/Expression.hpp>

namespace rask
{
namespace cst
{

class Optimizer
{
public:

    void optimize(ChainExpression& ce) const;
};

}
}

#endif // RASK_CST_OPTIMIZER_HPP
