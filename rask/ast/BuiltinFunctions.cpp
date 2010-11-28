// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/SymbolTable.hpp>
#include <rask/ast/BuiltinFunctions.hpp>

namespace rask
{
namespace ast
{

BuiltinFunctions::BuiltinFunctions()
    : print_(new BuiltinFunction("print", ast::VOID, 1)),
    getInt32_(new BuiltinFunction("getInt32", ast::INT32, 0))
{
}
    
void BuiltinFunctions::declare(SymbolTable& st)
{
    st.add(print_);
    st.add(getInt32_);
}
    
}
}
