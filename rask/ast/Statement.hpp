// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_STATEMENT_HPP
#define RASK_AST_STATEMENT_HPP

#include <boost/variant.hpp>
#include <boost/cstdint.hpp>
#include <rask/ast/VarDecl.hpp>

namespace rask
{
namespace ast
{

typedef boost::int32_t FunctionCall;
    
typedef boost::variant<VarDecl, FunctionCall> Statement;

}

}

#endif // RASK_AST_STATEMENT_HPP
