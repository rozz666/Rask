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
#include <llvm/BasicBlock.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <rask/cg/genFunctionIR.hpp>

namespace rask
{
namespace cg
{
 
IRCodeBuffer genFunctionIR(const ast::Function& f)
{
    llvm::LLVMContext ctx;
    llvm::Module *module = new llvm::Module("main", ctx);
    llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
    llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", func);
    cg::IRCodeBuffer buf;
    llvm::BitstreamWriter bw(buf);
    llvm::WriteBitcodeToStream(module, bw);
    return buf;
}
    
}
}

