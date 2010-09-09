// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_TREE_HPP
#define RASK_AST_TREE_HPP

#include <vector>
#include <rask/ast/FunctionDecl.hpp>

namespace rask
{  
namespace ast
{
 
class Tree
{
public:

    void add(SharedCustomFunction f)
    {
        functions_.push_back(f);
    }

    SharedCustomFunction function(std::size_t i) const
    {
        return functions_[i];
    }
    
    std::size_t functionCount() const
    {
        return functions_.size();
    }

private:

    std::vector<SharedCustomFunction> functions_;
};
    
}
}

#endif /* RASK_AST_TREE_HPP */
