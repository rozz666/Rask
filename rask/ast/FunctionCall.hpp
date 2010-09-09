// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTIONCALL_HPP
#define RASK_AST_FUNCTIONCALL_HPP

#include <vector>
#include <rask/ast/Function.hpp>
#include <rask/ast/Expression.hpp>

namespace rask
{
namespace ast
{

class FunctionCall
{
public:

    typedef std::vector<Expression> Arguments;
    
    FunctionCall(WeakFunction function, const Arguments& args) : function_(function), args_(args) { }

    WeakFunction function() const { return function_; }
    const Arguments& args() const { return args_; }

private:

    WeakFunction function_;
    Arguments args_;
};

}
}

#endif // RASK_AST_FUNCTIONCALL_HPP
