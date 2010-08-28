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
 
llvm::Function *CodeGenerator::genFunctionIR(const ast::Function& f, llvm::Module *module)
{
    llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(module->getContext()), false);
    llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", module);

    if (f.valueCount() > 0)
    {
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(module->getContext(), "entry", func);

        for (std::size_t i = 0; i != f.valueCount(); ++i)
        {
            llvm::ConstantInt *arg = llvm::ConstantInt::get(module->getContext(), llvm::APInt(32, f.value(i), true));
            llvm::CallInst *call = llvm::CallInst::Create(module->getFunction("_rask_print_int"), arg, "", entry);
        }
    }
    
    return func;
}
    
}
}

