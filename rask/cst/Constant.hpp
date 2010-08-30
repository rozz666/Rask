// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_CONSTANT_HPP
#define RASK_CST_CONSTANT_HPP

#include <boost/cstdint.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <rask/Position.hpp>

namespace rask
{
namespace cst
{

struct Constant
{
    Position position;
    boost::int32_t value;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Constant,
    (rask::Position, position)
    (boost::int32_t, value)
)
                          
#endif // RASK_CST_CONSTANT_HPP
