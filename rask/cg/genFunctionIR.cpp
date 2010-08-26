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
 
llvm::Function *CodeGenerator::genFunctionIR(const ast::Function& f, llvm::Module *module)
{
    llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(module->getContext()), false);
    llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", module);

    return func;
}
    
}
}

