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
#include <rask/cst/VariableDecl.hpp>
#include <rask/cst/Return.hpp>

namespace rask
{
namespace cst
{

typedef boost::variant<FunctionCall, VariableDecl, Return> Statement;

inline FunctionCall& getFunctionCall(Statement& s) { return boost::get<FunctionCall>(s); }
inline const FunctionCall& getFunctionCall(const Statement& s) { return boost::get<FunctionCall>(s); }

inline VariableDecl& getVariableDecl(Statement& s) { return boost::get<VariableDecl>(s); }
inline const VariableDecl& getVariableDecl(const Statement& s) { return boost::get<VariableDecl>(s); }

inline Return& getReturn(Statement& s) { return boost::get<Return>(s); }
inline const Return& getReturn(const Statement& s) { return boost::get<Return>(s); }

}
}

#endif // RASK_CST_STATEMENT_HPP
