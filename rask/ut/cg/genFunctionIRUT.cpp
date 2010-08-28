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
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

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
tut::factory tf("rask.cg.CodeGenerator.genFunctionIR");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::Function f;
    
    llvm::LLVMContext ctx;
    llvm::Module *module = new llvm::Module("testModule", ctx);

    llvm::Function *gf = cg::CodeGenerator().genFunctionIR(f, module);

    ensure("pointer type", llvm::isa<llvm::PointerType>(gf->getType()));
    ensure_equals("type", gf->getType()->getElementType(), llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false));
    ensure_equals("linkage", gf->getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("name", gf->getNameStr(), "main");
    ensure_equals("module", gf->getParent(), module);
    ensure("no code", gf->getBasicBlockList().empty());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ast::Function f;

    f.addValue(1);
    f.addValue(2);
    
    llvm::LLVMContext ctx;
    llvm::Module *module = new llvm::Module("testModule", ctx);

    cg::CodeGenerator cg;
    cg.declBuiltinFunctions(module);
    
    llvm::Function *gf = cg.genFunctionIR(f, module);
    
    ensure("pointer type", llvm::isa<llvm::PointerType>(gf->getType()));
    ensure_equals("type", gf->getType()->getElementType(), llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false));
    ensure_equals("linkage", gf->getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("name", gf->getNameStr(), "main");
    ensure_equals("module", gf->getParent(), module);

    ensure_equals("entry", gf->getBasicBlockList().size(), 1u);

    llvm::BasicBlock& bb = gf->getBasicBlockList().front();

    ensure_equals("bb name", bb.getNameStr(), "entry");
    ensure_equals("2 calls", bb.size(), 2u);

    llvm::BasicBlock::iterator it = bb.begin();

    ensure("call 1", llvm::isa<llvm::CallInst>(&*it));
    llvm::CallInst *call1 = llvm::cast<llvm::CallInst>(&*it);

    ensure("called 1", call1->getCalledFunction() == module->getFunction("_rask_print_int"));
    ensure_equals("num op 1", call1->getNumOperands(), 2u);
    ensure("value 1", call1->getOperand(1) == llvm::ConstantInt::get(ctx, llvm::APInt(32, f.value(0), true)));
    ensure("C cc 1", call1->getCallingConv() == llvm::CallingConv::C);

    ++it;
    
    ensure("call 2", llvm::isa<llvm::CallInst>(&*it));
    llvm::CallInst *call2 = llvm::cast<llvm::CallInst>(&*it);
    
    ensure("called 2", call2->getCalledFunction() == module->getFunction("_rask_print_int"));
    ensure_equals("num op 2", call2->getNumOperands(), 2u);
    ensure("value 2", call2->getOperand(1) == llvm::ConstantInt::get(ctx, llvm::APInt(32, f.value(1), true)));
    ensure("C cc 2", call2->getCallingConv() == llvm::CallingConv::C);
}

}
