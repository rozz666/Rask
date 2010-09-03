// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_IDENTIFIER_HPP
#define RASK_CST_IDENTIFIER_HPP

#include <string>
#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/Position.hpp>

namespace rask
{
namespace cst
{

struct Identifier
{
    Position position;
    std::string value;

    static Identifier create(Position position, std::string value)
    {
        Identifier id = { position, value };
        return id;
    }
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Identifier,
    (rask::Position, position)
    (std::string, value)
)

#endif /* RASK_CST_IDENTIFIER_HPP */