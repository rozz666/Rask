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
#include <rask/cg/Prefixes.hpp>

namespace rask
{
namespace cg
{

llvm::AllocaInst *CodeGenerator::genVariableDecl(const ast::VariableDecl& vd, llvm::BasicBlock& block)
{
    llvm::LLVMContext& ctx = block.getContext();
    llvm::Value *value = genValue(vd.value(), symbolTable_, block);
    llvm::AllocaInst *alloca = new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32), LOCAL_VAR_PREFIX + vd.var()->name().value, &block);
    new llvm::StoreInst(value, alloca, &block);

    symbolTable_.add(vd.var()->name(), alloca);
    
    return alloca;
}

}
}

    