// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

void CodeGenerator::declBuiltinFunctions(llvm::Module& module)
{
    llvm::LLVMContext& context = module.getContext();
    
    std::vector<const llvm::Type*> printIntArgs;
    printIntArgs.push_back(llvm::IntegerType::get(context, 32));
    llvm::FunctionType *printIntType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), printIntArgs, false);
    llvm::Function::Create(printIntType, llvm::Function::ExternalLinkage, "print", &module);
}
    
}
}