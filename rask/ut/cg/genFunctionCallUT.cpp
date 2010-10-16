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
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/SymbolTable.hpp>
#include <rask/ast/BuiltinFunction.hpp>
#include <rask/test/TUTAssert.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace tut
{

struct genFunctionCall_TestData
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    rask::cg::SymbolTable st;
    rask::cg::CodeGenerator cg;
    llvm::AllocaInst *a1;
    
    genFunctionCall_TestData()
        : module(new llvm::Module("testModule", ctx)), cg(st),
        a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32)))
    {
        cg.declBuiltinFunctions(*module);

        llvm::FunctionType *dummyType = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function::Create(dummyType, llvm::Function::ExternalLinkage, "dummy", &*module);
        
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

    ast::SharedBuiltinFunction print(new ast::BuiltinFunction("print", 1));
    ast::FunctionCall::Arguments args;
    args.push_back(ast::Constant(5));
    ast::FunctionCall fc(print, args);
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);

    ENSURE_EQUALS(block->size(), 1u);

    llvm::BasicBlock::iterator it = block->begin();
    
    ENSURE(&*it == call);
    ENSURE(call->getCalledFunction() == module->getFunction(print->name().value));
    ENSURE_EQUALS(call->getNumOperands(), 2u);
    ENSURE(call->getArgOperand(0) == llvm::ConstantInt::get(ctx, llvm::APInt(32, getConstant(args[0]), true)));
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::Identifier name;
    name.value = "asia";
    ast::VariableDecl vd(name, 1);
    ast::SharedBuiltinFunction print(new ast::BuiltinFunction("print", 1));
    ast::FunctionCall::Arguments args;
    args.push_back(vd.var());
    ast::FunctionCall fc(print, args);
    
    st.add(name, a1);
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);
    
    ENSURE_EQUALS(block->size(), 2u);
    
    llvm::BasicBlock::iterator it = block->begin();

    ENSURE(llvm::isa<llvm::LoadInst>(*it));
    llvm::LoadInst *load = llvm::cast<llvm::LoadInst>(&*it);

    ENSURE(load->getPointerOperand() == st.get(name));
    
    ++it;
    
    ENSURE(&*it == call);
    ENSURE(call->getCalledFunction() == module->getFunction(print->name().value));
    ENSURE_EQUALS(call->getNumOperands(), 2u);
    ENSURE(call->getArgOperand(0) == load);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction("print", 1));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);
    
    ENSURE_EQUALS(block->size(), 1u);
    
    llvm::BasicBlock::iterator it = block->begin();
    
    ENSURE(&*it == call);
    ENSURE(call->getCalledFunction() == module->getFunction(dummy->name().value));
    ENSURE_EQUALS(call->getNumOperands(), 1u);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

}
