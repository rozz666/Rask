// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_FUNCTIONCALL_HPP
#define RASK_CST_FUNCTIONCALL_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Expression.hpp>

namespace rask
{
namespace cst
{

struct FunctionCall
{
    Identifier function;
    std::vector<Expression> args;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::FunctionCall,
    (rask::cst::Identifier, function)
    (std::vector<rask::cst::Expression>, args)
)

#endif /* RASK_CST_FUNCTIONCALL_HPP */