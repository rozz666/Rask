// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTIONDECL_HPP
#define RASK_AST_FUNCTIONDECL_HPP

#include <rask/ast/CustomFunction.hpp>

namespace rask
{
namespace ast
{

class FunctionDecl
{
public:

    FunctionDecl(const cst::Identifier& name, ast::BasicType type)
        : function_(new CustomFunction(name, type)) { }

    SharedCustomFunction function() const { return function_; }

private:
    SharedCustomFunction function_;
};

}
}

#endif // RASK_AST_FUNCTIONDECL_HPP
