// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 
#include <rask/cg/genFunctionIR.hpp>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/BasicBlock.h>

namespace tut
{

struct genFunctionIR_TestData
{
};

typedef test_group<genFunctionIR_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.genFunctionIR");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::Function f;
    
    cg::IRCodeBuffer buf = cg::genFunctionIR(f);
    
    llvm::LLVMContext ctx;
    llvm::Module *module = new llvm::Module("main", ctx);
    llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
    llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", func);
    
    cg::IRCodeBuffer buf2;
    llvm::BitstreamWriter bw(buf2);
    llvm::WriteBitcodeToStream(module, bw);
    
    ensure_equals(buf.size(), buf2.size());
    ensure(buf == buf2);
   
}

}
