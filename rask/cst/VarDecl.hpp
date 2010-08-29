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

#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cst
{

struct VarDecl
{
    Identifier name;
    boost::optional<boost::int32_t> value;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::VarDecl,
    (rask::cst::Identifier, name)
    (boost::optional<boost::int32_t>, value)
)
                          
#endif // RASK_CST_VARDECL_HPP
