// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_BUILTINFUNCTIONS_HPP
#define RASK_AST_BUILTINFUNCTIONS_HPP

#include <rask/ast/BuiltinFunction.hpp>

namespace rask
{
namespace ast
{

class SymbolTable;
    
class BuiltinFunctions
{
public:

    BuiltinFunctions();
    
    void declare(SymbolTable& st);

private:

    SharedBuiltinFunction print_;
    SharedBuiltinFunction getInt32_;
    SharedBuiltinFunction operatorMinusInt32_;
    SharedBuiltinFunction operatorMinusInt32Int32_;
    SharedBuiltinFunction operatorPlusInt32Int32_;
};

}
}
#endif // RASK_AST_BUILTINFUNCTIONS_HPP
