// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_FUNCTION_HPP
#define RASK_CST_FUNCTION_HPP

#include <vector>
#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Identifier.hpp>
#include <rask/cst/Statement.hpp>

namespace rask
{
namespace cst
{

struct Function
{
    Identifier name;
    std::vector<Identifier> args;
    std::vector<Statement> stmts;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Function,
    (rask::cst::Identifier, name)
    (std::vector<rask::cst::Identifier>, args)
    (std::vector<rask::cst::Statement>, stmts)
)

#endif /* RASK_CST_FUNCTION_HPP */