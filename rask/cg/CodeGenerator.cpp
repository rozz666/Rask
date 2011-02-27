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
    SharedInstructionFactory instructionFactory,
    SharedTypeFactory typeFactory)
    : symbolTable_(symbolTable),
    basicBlockFactory_(basicBlockFactory),
    instructionFactory_(instructionFactory),
    typeFactory_(typeFactory)
{
    binaryOpMap_[BINARY_MINUS_NAME] = llvm::Instruction::Sub;
    binaryOpMap_[BINARY_PLUS_NAME] = llvm::Instruction::Add;
    binaryOpMap_[BINARY_MULT_NAME] = llvm::Instruction::Mul;
    binaryOpMap_[BINARY_DIV_NAME] = llvm::Instruction::SDiv;
    binaryOpMap_[BINARY_MOD_NAME] = llvm::Instruction::SRem;
}


}
}