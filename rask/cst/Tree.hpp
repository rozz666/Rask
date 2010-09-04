// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_TREE_HPP
#define RASK_CST_TREE_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Function.hpp>

namespace rask
{  
namespace cst
{
    
struct Tree
{
    std::vector<cst::Function> functions;
    Position end;
};
    
}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Tree,
    (std::vector<rask::cst::Function>, functions)
    (rask::Position, end)
)
                          
#endif /* RASK_CST_TREE_HPP */
