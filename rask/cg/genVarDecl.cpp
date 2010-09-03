// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <llvm/DerivedTypes.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

llvm::AllocaInst *CodeGenerator::genVarDecl(const ast::VarDecl& vd, llvm::BasicBlock& block)
{
    llvm::LLVMContext& ctx = block.getContext();
    llvm::AllocaInst *alloca = new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32), vd.var()->name().value, &block);
    llvm::ConstantInt *value = llvm::ConstantInt::get(ctx, llvm::APInt(32, vd.value(), true));
    new llvm::StoreInst(value, alloca, &block);

    symbolTable_.add(vd.var()->name(), alloca);
    
    return alloca;
}

}
}

    