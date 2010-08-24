// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/parseFunction.hpp>
#include <rask/ast/buildAST.hpp>

namespace rask
{
namespace ast
{
 
boost::optional<Tree> buildAST(const cst::Tree& cst, error::Logger& logger)
{
    if (boost::optional<Function> f = parseFunction(cst.main, logger))
    {
        Tree tree;
        tree.main = *f;
        return tree;
    }
    
    return boost::none;
}
    
}
}