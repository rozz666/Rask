// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/FunctionTable.hpp>
#include <rask/ast/BuiltinFunctions.hpp>
#include <rask/Operators.hpp>

namespace rask
{
namespace ast
{

BuiltinFunctions::BuiltinFunctions()
    : print_(new BuiltinFunction("print", ast::VOID, 1)),
    getInt32_(new BuiltinFunction("getInt32", ast::INT32, 0)),
    operatorMinusInt32_(new BuiltinFunction(UNARY_MINUS_NAME, ast::INT32, 1)),
    operatorMinusInt32Int32_(new BuiltinFunction(BINARY_MINUS_NAME, ast::INT32, 2)),
    operatorPlusInt32Int32_(new BuiltinFunction(BINARY_PLUS_NAME, ast::INT32, 2)),
    operatorMultInt32Int32_(new BuiltinFunction(BINARY_MULT_NAME, ast::INT32, 2)),
    operatorDivInt32Int32_(new BuiltinFunction(BINARY_DIV_NAME, ast::INT32, 2)),
    operatorModInt32Int32_(new BuiltinFunction(BINARY_MOD_NAME, ast::INT32, 2))
{
}
    
void BuiltinFunctions::declare(FunctionTable& ft)
{
    ft.add(print_);
    ft.add(getInt32_);
    ft.add(operatorMinusInt32_);
    ft.add(operatorMinusInt32Int32_);
    ft.add(operatorPlusInt32Int32_);
    ft.add(operatorMultInt32Int32_);
    ft.add(operatorDivInt32Int32_);
    ft.add(operatorModInt32Int32_);
}
    
}
}
