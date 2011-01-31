// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/CodeGenerator.hpp>
#include <rask/Operators.hpp>

namespace rask
{
namespace cg
{

CodeGenerator::CodeGenerator(
    SharedSymbolTable symbolTable,
    SharedBasicBlockFactory basicBlockFactory,
    SharedInstructionFactory instructionFactory)
    : symbolTable_(symbolTable),
    basicBlockFactory_(basicBlockFactory),
    instructionFactory_(instructionFactory)
{
    binaryOpMap_[BINARY_MINUS_NAME] = llvm::BinaryOperator::CreateNSWSub;
    binaryOpMap_[BINARY_PLUS_NAME] = llvm::BinaryOperator::CreateNSWAdd;
    binaryOpMap_[BINARY_MULT_NAME] = llvm::BinaryOperator::CreateNSWMul;
    binaryOpMap_[BINARY_DIV_NAME] = llvm::BinaryOperator::CreateSDiv;
    binaryOpMap_[BINARY_MOD_NAME] = llvm::BinaryOperator::CreateSRem;
}


}
}