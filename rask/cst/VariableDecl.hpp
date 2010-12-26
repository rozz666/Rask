// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_VARDECL_HPP
#define RASK_CST_VARDECL_HPP

#include <boost/optional.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <rask/cst/Identifier.hpp>
#include <rask/cst/Expression.hpp>

namespace rask
{
namespace cst
{

struct VariableDecl
{
    Identifier name;
    boost::optional<ChainExpression> value;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::VariableDecl,
    (rask::cst::Identifier, name)
    (boost::optional<rask::cst::ChainExpression>, value)
)
                          
#endif // RASK_CST_VARDECL_HPP
