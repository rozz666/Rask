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

typedef boost::variant<boost::int32_t, SharedVariable> FunctionCall;

inline boost::int32_t& getInt32(FunctionCall& fc) { return boost::get<boost::int32_t>(fc); }
inline const boost::int32_t& getInt32(const FunctionCall& fc) { return boost::get<boost::int32_t>(fc); }

inline SharedVariable& getVariable(FunctionCall& fc) { return boost::get<SharedVariable>(fc); }
inline const SharedVariable& getVariable(const FunctionCall& fc) { return boost::get<SharedVariable>(fc); }

typedef boost::variant<VarDecl, FunctionCall> Statement;

inline FunctionCall& getFunctionCall(Statement& s) { return boost::get<FunctionCall>(s); }
inline const FunctionCall& getFunctionCall(const Statement& s) { return boost::get<FunctionCall>(s); }

inline VarDecl& getVarDecl(Statement& s) { return boost::get<VarDecl>(s); }
inline const VarDecl& getVarDecl(const Statement& s) { return boost::get<VarDecl>(s); }

}

}

#endif // RASK_AST_STATEMENT_HPP
