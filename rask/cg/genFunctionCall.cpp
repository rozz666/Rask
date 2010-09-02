// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <llvm/LLVMContext.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

llvm::CallInst *CodeGenerator::genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block, llvm::Module& module)
{
    llvm::ConstantInt *arg = llvm::ConstantInt::get(module.getContext(), llvm::APInt(32, fc, true));
    return llvm::CallInst::Create(module.getFunction("_rask_print_int"), arg, "", &block);
}
    
    
}
}