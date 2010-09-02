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
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace tut
{

struct genFunctionCall_TestData
{
    llvm::LLVMContext ctx;
    std::auto_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    rask::cg::CodeGenerator cg;
    
    genFunctionCall_TestData() : module(new llvm::Module("testModule", ctx))
    {
        cg.declBuiltinFunctions(*module);

        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }
};

typedef test_group<genFunctionCall_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genFunctionCall");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::FunctionCall fc(5);
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);

    ensure_equals("1 instr", block->size(), 1u);

    llvm::BasicBlock::iterator it = block->begin();
    
    ensure("call", &*it == call);
    
    ensure("called", call->getCalledFunction() == module->getFunction("_rask_print_int"));
    ensure_equals("num ops", call->getNumOperands(), 2u);
    ensure("value", call->getOperand(1) == llvm::ConstantInt::get(ctx, llvm::APInt(32, fc, true)));
    ensure("C cc", call->getCallingConv() == llvm::CallingConv::C);
}

}
