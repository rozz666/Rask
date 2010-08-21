// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_PARSEFUNCTION_HPP
#define RASK_AST_PARSEFUNCTION_HPP

#include <boost/optional.hpp>
#include <rask/cst/Function.hpp>
#include <rask/ast/Function.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace ast
{

boost::optional<Function> parseFunction(const cst::Function& cf, error::Logger& logger);

}
}

#endif /* RASK_AST_PARSEFUNCTION_HPP */