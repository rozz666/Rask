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
    rask::ast::Function f;
    llvm::LLVMContext ctx;
    std::auto_ptr<llvm::Module> module;
    rask::cg::CodeGenerator cg;
    llvm::BasicBlock *entry;
    
    genFunctionIR_TestData() : module(new llvm::Module("testModule", ctx))
    {
        cg.declBuiltinFunctions(*module);
    }

    void ensureMainDef(llvm::Function *gf)
    {
        ensure("pointer type", llvm::isa<llvm::PointerType>(gf->getType()));
        ensure_equals("type", gf->getType()->getElementType(), llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false));
        ensure_equals("linkage", gf->getLinkage(), llvm::Function::ExternalLinkage);
        ensure_equals("name", gf->getNameStr(), "main");
        ensure_equals("module", gf->getParent(), module.get());
        ensure_equals("entry", gf->getBasicBlockList().size(), 1u);
        entry = &gf->getBasicBlockList().front();
        ensure_equals("entry name", entry->getNameStr(), "entry");
    }

    void ensurePrintCall(const std::string& name, llvm::Instruction *instr, int32_t arg)
    {
        ensure(name + ": call", llvm::isa<llvm::CallInst>(instr));
        llvm::CallInst *call = llvm::cast<llvm::CallInst>(instr);
        
        ensure(name + ": called", call->getCalledFunction() == module->getFunction("_rask_print_int"));
        ensure_equals(name + ": num ops", call->getNumOperands(), 2u);
        ensure(name + ": value", call->getOperand(1) == llvm::ConstantInt::get(ctx, llvm::APInt(32, arg, true)));
        ensure(name + ": C cc", call->getCallingConv() == llvm::CallingConv::C);
    }
};

typedef test_group<genFunctionIR_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    llvm::Function *gf = cg.genFunction(f, *module);

    ensureMainDef(gf);
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    f.addValue(1);
    f.addValue(2);

    llvm::Function *gf = cg.genFunction(f, *module);

    ensureMainDef(gf);
    ensure_equals("3 instructions", entry->size(), 3u);

    llvm::BasicBlock::iterator it = entry->begin();

    ensurePrintCall("1", &*it, f.value(0)); ++it;
    ensurePrintCall("2", &*it, f.value(1)); ++it;
    ensure("ret", llvm::isa<llvm::ReturnInst>(*it));
}

}
