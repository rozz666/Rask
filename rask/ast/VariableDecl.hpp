// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_VARDECL_HPP
#define RASK_AST_VARDECL_HPP

#include <boost/cstdint.hpp>
#include <rask/ast/Variable.hpp>
#include <rask/ast/Expression.hpp>

namespace rask
{
namespace ast
{

class VariableDecl
{
public:

    VariableDecl(SharedVariable var, const ast::Expression& value) : var_(var), value_(value) { }

    SharedVariable var() const { return var_; }
    const ast::Expression& value() const { return value_; }

private:

    SharedVariable var_;
    ast::Expression value_;
};

}
}

#endif // RASK_AST_VARDECL_HPP
