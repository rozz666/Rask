// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_RETURN_HPP
#define RASK_CST_RETURN_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <rask/Position.hpp>
#include <rask/cst/Expression.hpp>

namespace rask
{
namespace cst
{

struct Return
{
    Position position;
    Expression value;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Return,
    (rask::Position, position)
    (rask::cst::Expression, value)
)

#endif // RASK_CST_RETURN_HPP
