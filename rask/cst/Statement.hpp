// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RASK_CST_STATEMENT_HPP
#define RASK_CST_STATEMENT_HPP

#include <boost/variant.hpp>
#include <rask/cst/FunctionCall.hpp>
#include <rask/cst/VarDecl.hpp>

namespace rask
{
namespace cst
{

typedef boost::variant<FunctionCall, VarDecl> Statement;

}
}

#endif // RASK_CST_STATEMENT_HPP
