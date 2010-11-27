// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/foreach.hpp>
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

void CodeGenerator::declFunction(const ast::CustomFunction& cf, llvm::Module& module)
{
    std::vector<const llvm::Type *> fArgs(cf.argCount(), llvm::IntegerType::get(module.getContext(), 32));
    const llvm::Type *returnType =
        (cf.type() == ast::VOID) ?
            llvm::Type::getVoidTy(module.getContext()) :
            llvm::IntegerType::get(module.getContext(), 32);
    llvm::FunctionType *fType = llvm::FunctionType::get(returnType, fArgs, false);
    llvm::Function *f = llvm::Function::Create(fType, llvm::Function::ExternalLinkage, cf.name().value, &module);

    BOOST_FOREACH(llvm::Argument& arg, f->getArgumentList())
    {
        arg.setName("a_" + cf.arg(arg.getArgNo())->name().value);
    }
}

}
}