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
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

void CodeGenerator::declFunction(const ast::CustomFunction& f, llvm::Module& module)
{
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(module.getContext()), false);
    llvm::Function::Create(fType, llvm::Function::ExternalLinkage, f.name().value, &module);
}

}
}