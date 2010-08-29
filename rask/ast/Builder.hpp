// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_BUILDER_HPP
#define RASK_AST_BUILDER_HPP

#include <boost/optional.hpp>
#include <rask/ast/Tree.hpp>
#include <rask/cst/Tree.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace ast
{

class Builder
{
public:
        
    virtual boost::optional<Function> buildFunction(const cst::Function& f, error::Logger& el);
    virtual boost::optional<Tree> buildTree(const cst::Tree& cst, error::Logger& el);
};

}
}

#endif // RASK_AST_BUILDER_HPP
